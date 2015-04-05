#include <stdio.h>
#include <stdlib.h>
#include <SSPComs.h>
#include "ssp_helpers.h"
#include "main.h"
#include "ssp.h"
#include <async.h>

static const unsigned long long encryptionKey = 0x123456701234567LL;

static int sspPort = -1;
static SSP_COMMAND sspC;

void parse_poll(SSP_POLL_DATA6 *poll);

bool sspConnectToValidator(const char * const sspAddress) {
	SSP6_SETUP_REQUEST_DATA setup_req;
	fprintf(stderr, "Connecting to validator...\n");

	sspC.Timeout = 1000;
	sspC.EncryptionStatus = NO_ENCRYPTION;
	sspC.RetryLevel = 3;
	sspC.BaudRate = 9600;

	if((sspPort = OpenSSPPort(sspAddress)) == -1) {
		fprintf(stderr, "Failed opening port %s.\n", sspAddress);
		return false;
	}

	// check validator is present
	if(ssp6_sync(&sspC) != SSP_RESPONSE_OK) {
		fprintf(stderr, "NO VALIDATOR FOUND\n");
		return false;
	}
	fprintf(stderr, "Validator Found\n");

	// try to setup encryption using the default key
	if(ssp6_setup_encryption(&sspC, encryptionKey) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Encryption Failed\n");
		return false;
	}
	fprintf(stderr, "SSP Encryption Setup\n");

	// Make sure we are using ssp version 6
	if(ssp6_host_protocol(&sspC, 0x06) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Host Protocol Failed\n");
		return false;
	}

	// Collect some information about the validator
	if(ssp6_setup_request(&sspC, &setup_req) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Setup Request Failed\n");
		return false;
	}
	fprintf(stderr, "Firmware: %s\n", setup_req.FirmwareVersion);
	fprintf(stderr, "Channels:\n");
	for(int i=0; i<setup_req.NumberOfChannels; i++) {
		printf("channel %d: %d %s\n", i+1, setup_req.ChannelData[i].value, setup_req.ChannelData[i].cc);
	}

	// enable the payout unit
	if(ssp6_enable_payout(&sspC, setup_req.UnitType) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Enable Failed\n");
		return false;
	}

	// set the inhibits (enable all note acceptance)
	if(ssp6_set_inhibits(&sspC,0xFF,0xFF) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Inhibits Failed\n");
		return false;
	}

	// enable the validator
	if(ssp6_enable(&sspC) != SSP_RESPONSE_OK) {
		fprintf(stderr, "SSP Enable Failed\n");
		return false;
	}
	return true;
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
			terminate();
			break;
		case 0xFA:
			// The validator has responded with key not set, so we should try to negotiate one
			if(ssp6_setup_encryption(&sspC, encryptionKey) != SSP_RESPONSE_OK) {
				fprintf(stderr, "SSP Encryption Failed\n");
				terminate();
			} else {
				fprintf(stderr, "SSP Encryption Setup\n");
			}
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
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'read'}");
			} else {
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'reading'}");
			}
			break;
		case SSP_POLL_CREDIT:
			// The note which was in escrow has been accepted
			printf("Credit %ld %s\n", poll->events[i].data1, poll->events[i].cc);
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'credit','channel':%ld}", poll->events[i].data1);
			break;
		case SSP_POLL_INCOMPLETE_PAYOUT:
			// the validator shutdown during a payout, this event is reporting that some value remains to payout
			printf("Incomplete payout %ld of %ld %s\n", poll->events[i].data1, poll->events[i].data2, poll->events[i].cc);
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'incomplete payout','dispensed':%ld,'requested':%ld}", poll->events[i].data1, poll->events[i].data2);
			break;
		case SSP_POLL_INCOMPLETE_FLOAT:
			// the validator shutdown during a float, this event is reporting that some value remains to float
			printf("Incomplete float %ld of %ld %s\n", poll->events[i].data1, poll->events[i].data2, poll->events[i].cc);
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'incomplete float','floated':%ld,'requested':%ld}", poll->events[i].data1, poll->events[i].data2);
			break;
		case SSP_POLL_REJECTING:
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'rejecting'}");
			break;
		case SSP_POLL_REJECTED:
			// The note was rejected
			printf("Note Rejected\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'rejected'}");
			break;
		case SSP_POLL_STACKING:
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'stacking'}");
			break;
		case SSP_POLL_STORED:
			// The note has been stored in the payout unit
			printf("Stored\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'stored'}");
			break;
		case SSP_POLL_STACKED:
			// The note has been stacked in the cashbox
			printf("Stacked\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'stacked'}");
			break;
		case SSP_POLL_SAFE_JAM:
			printf("Safe Jam\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'safe jam'}");
			break;
		case SSP_POLL_UNSAFE_JAM:
			printf("Unsafe Jam\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'unsafe jam'}");
			break;
		case SSP_POLL_DISABLED:
			// The validator has been disabled
			printf("DISABLED\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'disabled'}");
			break;
		case SSP_POLL_FRAUD_ATTEMPT:
			// The validator has detected a fraud attempt
			printf("Fraud Attempt %ld %s\n", poll->events[i].data1, poll->events[i].cc);
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'fraud attempt','dispensed':%ld}", poll->events[i].data1);
			break;
		case SSP_POLL_STACKER_FULL:
			// The cashbox is full
			printf("Stacker Full\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'stacker full'}");
			break;
		case SSP_POLL_CASH_BOX_REMOVED:
			// The cashbox has been removed
			printf("Cashbox Removed\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'cashbox removed'}");
			break;
		case SSP_POLL_CASH_BOX_REPLACED:
			// The cashbox has been replaced
			printf("Cashbox Replaced\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'cashbox replaced'}");
			break;
		case SSP_POLL_CLEARED_FROM_FRONT:
			// A note was in the notepath at startup and has been cleared from the front of the validator
			printf("Cleared from front\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'cleared from front'}");
			break;
		case SSP_POLL_CLEARED_INTO_CASHBOX:
			// A note was in the notepath at startup and has been cleared into the cashbox
			printf("Cleared Into Cashbox\n");
			redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'cleared into cashbox'}");
			break;
		case SSP_POLL_CALIBRATION_FAIL:
			// the hopper calibration has failed. An extra byte is available with an error code.
			printf("Calibration fail: ");

			switch(poll->events[i].data1) {
			case NO_FAILUE:
				printf ("No failure\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'no error'}");
				break;
			case SENSOR_FLAP:
				printf ("Optical sensor flap\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'sensor flap'}");
				break;
			case SENSOR_EXIT:
				printf ("Optical sensor exit\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'sensor exit'}");
				break;
			case SENSOR_COIL1:
				printf ("Coil sensor 1\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'sensor coil 1'}");
				break;
			case SENSOR_COIL2:
				printf ("Coil sensor 2\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'sensor coil 2'}");
				break;
			case NOT_INITIALISED:
				printf ("Unit not initialised\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'not initialized'}");
				break;
			case CHECKSUM_ERROR:
				printf ("Data checksum error\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'calibration fail','error':'checksum error'}");
				break;
			case COMMAND_RECAL:
				printf ("Recalibration by command required\n");
				redisAsyncCommand(db, NULL, NULL, "PUBLISH validator {'event':'recalibrating'}");
				ssp6_run_calibration(&sspC);
				break;
			}
			break;
		}
	}
}

sspResult sspEmpty(void) {
	sspC.CommandDataLength = 1;
	sspC.CommandData[0] = SSP_CMD_EMPTY;

	if(send_ssp_command(&sspC) == 0) {
		return sspResultTimeout;
	}
	return (sspResult)sspC.ResponseData[0];
}

sspResult sspGetAllLevels(uint8_t *count, struct SSPDenomination **levels) {
	static struct SSPDenomination levelsBuffer[28]; // can't be more than 28, because the maximum response size is 255 bytes, and a single record is 9 bytes long
	sspC.CommandDataLength = 1;
	sspC.CommandData[0] = SSP_CMD_GET_ALL_LEVELS;

	if(send_ssp_command(&sspC) == 0) {
		return sspResultTimeout;
	}
	if(sspC.ResponseData[0] != SSP_RESPONSE_OK) {
		return (sspResult)sspC.ResponseData[0];
	}
	*count = sspC.ResponseData[1];
	if(*count > 28) {
		*count = 28;
	}
	*levels = levelsBuffer;
	for(uint8_t i = 0; i < *count; ++i) {
		levelsBuffer[i].count = sspC.ResponseData[1+i*9+1] << 8 | (uint16_t)sspC.ResponseData[1+i*9+0];
		levelsBuffer[i].value = sspC.ResponseData[1+i*9+5] << 24 | sspC.ResponseData[1+i*9+4] << 16 | sspC.ResponseData[1+i*9+3] << 8 | (uint16_t)sspC.ResponseData[1+i*9+2];
		// ignore currency
	}
	return sspResultOk;
}

sspResult sspPayout(int value) {
	return sspResultOk;
}

sspResult sspPayoutByDenomination(unsigned count, const struct SSPDenomination * const denominationList, bool test) {
	return sspResultOk;
}

sspResult sspSetBezel(unsigned char r, unsigned char g, unsigned char b) {
	return sspResultOk;
}
