#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <SSPComs.h>
#include "ssp_helpers.h"

static const unsigned long long encryptionKey = 0x123456701234567LL;

static int sspPort = -1;
static SSP_COMMAND sspC;

void parse_poll(SSP_POLL_DATA6 *poll);

void sspConnectToValidator(const char * const sspAddress) {
	SSP6_SETUP_REQUEST_DATA setup_req;
	fprintf(stderr, "Connecting to validator...\n");

	sspC.Timeout = 1000;
	sspC.EncryptionStatus = NO_ENCRYPTION;
	sspC.RetryLevel = 3;
	sspC.BaudRate = 9600;

	if((sspPort = OpenSSPPort(sspAddress)) == -1) {
		fprintf(stderr, "Failed opening port %s.\n", sspAddress);
		exit(errno);
	}

	// check validator is present
	if(ssp6_sync(&sspC) != SSP_RESPONSE_OK) {
		fprintf(stderr, "NO VALIDATOR FOUND\n");
		exit(3);
	}
	fprintf(stderr, "Validator Found\n");

	// try to setup encryption using the default key
	if(ssp6_setup_encryption(&sspC, encryptionKey) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Encryption Failed\n");
		exit(3);
	}
	fprintf(stderr, "SSP Encryption Setup\n");

	// Make sure we are using ssp version 6
	if(ssp6_host_protocol(&sspC, 0x06) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Host Protocol Failed\n");
		exit(3);
	}

	// Collect some information about the validator
	if(ssp6_setup_request(&sspC, &setup_req) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Setup Request Failed\n");
		exit(3);
	}
	fprintf(stderr, "Firmware: %s\n", setup_req.FirmwareVersion);
	fprintf(stderr, "Channels:\n");
	for(int i=0; i<setup_req.NumberOfChannels; i++) {
		printf("channel %d: %d %s\n", i+1, setup_req.ChannelData[i].value, setup_req.ChannelData[i].cc);
	}

	// enable the payout unit
	if(ssp6_enable_payout(&sspC, setup_req.UnitType) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Enable Failed\n");
		exit(3);
	}

	// set the inhibits (enable all note acceptance)
	if(ssp6_set_inhibits(&sspC,0xFF,0xFF) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Inhibits Failed\n");
		exit(3);
	}

	// enable the validator
	if(ssp6_enable(&sspC) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Enable Failed\n");
		exit(3);
	}
}

void sspPoll(int fd, short event, void *arg) {
	SSP_RESPONSE_ENUM rsp_status;
	SSP_POLL_DATA6 poll;
	switch(rsp_status = ssp6_poll(&sspC, &poll)) {
		case SSP_RESPONSE_OK:
			parse_poll(&poll);
			break;
		case SSP_RESPONSE_TIMEOUT:
			fprintf(stderr, "SSP Poll Timeout\n");
			exit(3);
		case 0xFA:
			// The validator has responded with key not set, so we should try to negotiate one
			if(ssp6_setup_encryption(&sspC, encryptionKey) != SSP_RESPONSE_OK) {
				fprintf(stderr, "SSP Encryption Failed\n");
				exit(3);
			}
			fprintf(stderr, "SSP Encryption Setup\n");
			break;
		default:
			fprintf(stderr, "SSP Poll Error: 0x%x\n", rsp_status);
			break;
	}
}

int send_ssp_command(SSP_COMMAND *sspC) {
	return SSPSendCommand(sspPort, sspC);
}

int negotiate_ssp_encryption(SSP_COMMAND *sspC, SSP_FULL_KEY * hostKey){
	return NegotiateSSPEncryption(sspPort, sspC->SSPAddress, hostKey);
}

void sspCleanup(void) {
	if(sspPort != -1) {
		CloseSSPPort(sspPort);
		sspPort = -1;
	}
}

// parse the validators response to the poll command. the SSP_POLL_DATA6 structure has an
// array of structures which contain values and country codes
void parse_poll(SSP_POLL_DATA6 * poll) {
	for(int i = 0; i < poll->event_count; ++i) {
		switch(poll->events[i].event) {
		case SSP_POLL_RESET:
			printf("Unit Reset\n");
			// Make sure we are using ssp version 6
			if(ssp6_host_protocol(&sspC, 0x06) != SSP_RESPONSE_OK) {
				fprintf(stderr, "SSP Host Protocol Failed\n");
				exit(3);
			}
			break;
		case SSP_POLL_READ:
			// the 'read' event contains 1 data value, which if >0 means a note has been validated and is in escrow
			if(poll->events[i].data1 > 0) {
				printf("Note Read %ld %s\n", poll->events[i].data1, poll->events[i].cc);
			}
			break;
		case SSP_POLL_CREDIT:
			// The note which was in escrow has been accepted
			printf("Credit %ld %s\n", poll->events[i].data1, poll->events[i].cc);
			break;
		case SSP_POLL_INCOMPLETE_PAYOUT:
			// the validator shutdown during a payout, this event is reporting that some value remains to payout
			printf("Incomplete payout %ld of %ld %s\n", poll->events[i].data1, poll->events[i].data2, poll->events[i].cc);
			break;
		case SSP_POLL_INCOMPLETE_FLOAT:
			// the validator shutdown during a float, this event is reporting that some value remains to float
			printf("Incomplete float %ld of %ld %s\n", poll->events[i].data1, poll->events[i].data2, poll->events[i].cc);
			break;
		case SSP_POLL_REJECTING:
			break;
		case SSP_POLL_REJECTED:
			// The note was rejected
			printf("Note Rejected\n");
			break;
		case SSP_POLL_STACKING:
			break;
		case SSP_POLL_STORED:
			// The note has been stored in the payout unit
			printf("Stored\n");
			break;
		case SSP_POLL_STACKED:
			// The note has been stacked in the cashbox
			printf("Stacked\n");
			break;
		case SSP_POLL_SAFE_JAM:
			printf("Safe Jam\n");
			break;
		case SSP_POLL_UNSAFE_JAM:
			printf("Unsafe Jam\n");
			break;
		case SSP_POLL_DISABLED:
			// The validator has been disabled
			printf("DISABLED\n");
			break;
		case SSP_POLL_FRAUD_ATTEMPT:
			// The validator has detected a fraud attempt
			printf("Fraud Attempt %ld %s\n", poll->events[i].data1, poll->events[i].cc);
			break;
		case SSP_POLL_STACKER_FULL:
			// The cashbox is full
			printf("Stacker Full\n");
			break;
		case SSP_POLL_CASH_BOX_REMOVED:
			// The cashbox has been removed
			printf("Cashbox Removed\n");
			break;
		case SSP_POLL_CASH_BOX_REPLACED:
			// The cashbox has been replaced
			printf("Cashbox Replaced\n");
			break;
		case SSP_POLL_CLEARED_FROM_FRONT:
			// A note was in the notepath at startup and has been cleared from the front of the validator
			printf("Cleared from front\n");
			break;
		case SSP_POLL_CLEARED_INTO_CASHBOX:
			// A note was in the notepath at startup and has been cleared into the cashbox
			printf("Cleared Into Cashbox\n");
			break;
		case SSP_POLL_CALIBRATION_FAIL:
			// the hopper calibration has failed. An extra byte is available with an error code.
			printf("Calibration fail: ");

			switch(poll->events[i].data1) {
			case NO_FAILUE:
				printf ("No failure\n");
				break;
			case SENSOR_FLAP:
				printf ("Optical sensor flap\n");
				break;
			case SENSOR_EXIT:
				printf ("Optical sensor exit\n");
				break;
			case SENSOR_COIL1:
				printf ("Coil sensor 1\n");
				break;
			case SENSOR_COIL2:
				printf ("Coil sensor 2\n");
				break;
			case NOT_INITIALISED:
				printf ("Unit not initialised\n");
				break;
			case CHECKSUM_ERROR:
				printf ("Data checksum error\n");
				break;
			case COMMAND_RECAL:
				printf ("Recalibration by command required\n");
				ssp6_run_calibration(&sspC);
				break;
			}
			break;
		}
	}
}