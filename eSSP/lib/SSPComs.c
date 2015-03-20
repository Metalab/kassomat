
#include <sys/time.h>
#include <time.h>
#include "../inc/SSPComs.h"
#include "../inc/ssp_defines.h"
#include "Encryption.h"
#include "serialfunc.h"
#include "ITLSSPProc.h"



extern unsigned int encPktCount[MAX_SSP_PORT];
extern unsigned char sspSeq[MAX_SSP_PORT];

int CompileSSPCommand(SSP_COMMAND* cmd,SSP_TX_RX_PACKET* ss)
{

	int i,j;
	unsigned short crc;
	unsigned char tBuffer[255];

	ss->rxPtr = 0;
	for(i = 0; i < 255; i++)
		ss->rxData[i] = 0x00;






	/* for sync commands reset the deq bit   */
	if(cmd->CommandData[0] == SSP_CMD_SYNC)
		sspSeq[cmd->SSPAddress] = 0x80;



	/* is this a encrypted packet  */
	if(cmd->EncryptionStatus){

		if(!EncryptSSPPacket(cmd->SSPAddress ,cmd->CommandData,cmd->CommandData,&cmd->CommandDataLength,&cmd->CommandDataLength,(unsigned long long*)&cmd->Key))
			return 0;

	}

	/* create the packet from this data   */
	ss->CheckStuff = 0;
	ss->SSPAddress = cmd->SSPAddress;
	ss->rxPtr = 0;
	ss->txPtr = 0;
	ss->rxBufferLength = 3;
	ss->txBufferLength = cmd->CommandDataLength + 5;  /* the full ssp packet length   */
	ss->txData[0] = SSP_STX;					/* ssp packet start   */
	ss->txData[1] = cmd->SSPAddress | sspSeq[cmd->SSPAddress];  /* the address/seq bit */
	ss->txData[2] = cmd->CommandDataLength;    /* the data length only (always > 0)  */
	for(i = 0; i < cmd->CommandDataLength; i++)  /* add the command data  */
		ss->txData[3 + i] = cmd->CommandData[i];
	/* calc the packet CRC  (all bytes except STX)   */
	crc = cal_crc_loop_CCITT_A(ss->txBufferLength - 3,&ss->txData[1] ,CRC_SSP_SEED,CRC_SSP_POLY);
	ss->txData[3 + cmd->CommandDataLength] = (unsigned char)(crc & 0xFF);
	ss->txData[4 + cmd->CommandDataLength] = (unsigned char)((crc >> 8) & 0xFF);


	/* we now need to 'byte stuff' this buffered data   */
	j = 0;
	tBuffer[j++] = ss->txData[0];
	for(i = 1; i < ss->txBufferLength; i++){
		tBuffer[j] = ss->txData[i];
		if (ss->txData[i] ==	SSP_STX){
			tBuffer[++j] = SSP_STX;   /* SSP_STX found in data so add another to 'stuff it'  */
		}
		j++;
	}
	for(i = 0; i < j; i++)
		ss->txData[i] = tBuffer[i];
	ss->txBufferLength  = j;

	return 1;
}

/*
Name: SSPSendCommand
Inputs:
    SSP_PORT The port handle (returned from OpenSSPPort) of the port to use
    SSP_COMMAND The command structure to be used.
Return:
    1 on success
    0 on failure
Notes:
    In the ssp_command structure:
    EncryptionStatus,SSPAddress,Timeout,RetryLevel,CommandData,CommandDataLength (and Key if using encrpytion) must be set before calling this function
    ResponseStatus,ResponseData,ResponseDataLength will be altered by this function call.
*/
int  SSPSendCommand(const SSP_PORT port, SSP_COMMAND* cmd)
{
    SSP_TX_RX_PACKET ssp;
	clock_t txTime,currentTime,rxTime;
	int i;
	unsigned char encryptLength;
	unsigned short crcR;
	unsigned char buffer;
	unsigned char tData[255];
	unsigned char retry;
	unsigned int slaveCount;
    /* complie the SSP packet and check for errors  */
    if(!CompileSSPCommand(cmd,&ssp )){
        cmd->ResponseStatus = SSP_PACKET_ERROR;
        return 0;
    }

    retry = cmd->RetryLevel;
    /* transmit the packet    */
    do{
        ssp.NewResponse = 0;  /* set flag to wait for a new reply from slave   */
        if (WriteData(ssp.txData,ssp.txBufferLength,port) == 0)
        {
        //if(WritePort(&ssp) != TRUE){
            cmd->ResponseStatus = PORT_ERROR;
            return 0;
        }

        /* wait for out reply   */
        cmd->ResponseStatus = SSP_REPLY_OK;
        txTime = GetClockMs();
        while(!ssp.NewResponse){
            /* check for reply timeout   */
            currentTime = GetClockMs();
            if(currentTime - txTime > cmd->Timeout){
                cmd->ResponseStatus = SSP_CMD_TIMEOUT;
                break;
            }
            while (BytesInBuffer(port) > 0)
            {
                ReadData(port,&buffer,1);
                SSPDataIn(buffer,&ssp);
            }
        }

        if(cmd->ResponseStatus == SSP_REPLY_OK)
            break;

        retry--;
    }while(retry > 0);


    rxTime = GetClockMs();

    if(cmd->ResponseStatus == SSP_CMD_TIMEOUT){
            cmd->ResponseData[0] = SSP_RESPONSE_TIMEOUT;
            return 0;
    }



    /* load the command structure with ssp packet data   */
    if(ssp.rxData[3] == SSP_STEX){   /* check for encrpted packet    */
        encryptLength = ssp.rxData[2] - 1;
        DecryptSSPPacket(&ssp.rxData[4],&ssp.rxData[4],&encryptLength,&encryptLength,(unsigned long long*)&cmd->Key);
        /* check the checsum    */
        crcR = cal_crc_loop_CCITT_A(encryptLength - 2,&ssp.rxData[4] ,CRC_SSP_SEED,CRC_SSP_POLY);
        if((unsigned char)(crcR & 0xFF) != ssp.rxData[ssp.rxData[2] + 1] || (unsigned char)((crcR >> 8) & 0xFF) != ssp.rxData[ssp.rxData[2] + 2]){
            cmd->ResponseStatus = SSP_PACKET_ERROR;
            return 0;
        }
        /* check the slave count against the host count  */
        slaveCount = 0;
        for(i = 0; i < 4; i++)
            slaveCount += (unsigned int)(ssp.rxData[5 + i]) << (i*8);
        /* no match then we discard this packet and do not act on it's info  */
        if(slaveCount != encPktCount[cmd->SSPAddress] ){
            cmd->ResponseStatus = SSP_PACKET_ERROR;
            return 0;
        }

        /* restore data for correct decode  */
        ssp.rxBufferLength = ssp.rxData[4] + 5;
        tData[0] = ssp.rxData[0];
        tData[1] = ssp.rxData[1];
        tData[2] = ssp.rxData[4];
        for(i = 0; i < ssp.rxData[4]; i++)
            tData[3 + i] = ssp.rxData[9 + i];
        crcR = cal_crc_loop_CCITT_A(ssp.rxBufferLength - 3,&tData[1] ,CRC_SSP_SEED,CRC_SSP_POLY);
        tData[3 + ssp.rxData[4]] = (unsigned char)(crcR & 0xFF);
        tData[4 + ssp.rxData[4]] = (unsigned char)((crcR >> 8) & 0xFF);
        for(i = 0; i < ssp.rxBufferLength; i++)
            ssp.rxData[i] = tData[i];

        /* for decrypted resonse with encrypted command, increment the counter here  */
    //	if(!cmd->EncryptionStatus)
          //encPktCount[cmd->SSPAddress]++;


    }

    /*for(i = 0; i < ssp.rxBufferLength; i++)
            printf("%x ", ssp.rxData[i]);
        printf("\n");*/
    cmd->ResponseDataLength = ssp.rxData[2];
    for(i = 0; i < cmd->ResponseDataLength; i++)
        cmd->ResponseData[i] = ssp.rxData[i + 3];


    /* alternate the seq bit   */
    if(sspSeq[cmd->SSPAddress] == 0x80)
        sspSeq[cmd->SSPAddress] = 0;
    else
        sspSeq[cmd->SSPAddress] = 0x80;


	/* terminate the thread function   */
	cmd->ResponseStatus = SSP_REPLY_OK;

	return 1;
}

clock_t GetClockMs()
{
    clock_t test;
    struct timeval tv;
    gettimeofday(&tv, 0);
    test = tv.tv_sec * 1000;
    test += (tv.tv_usec)/1000;
    return test;
}


void SSPDataIn(unsigned char RxChar, SSP_TX_RX_PACKET* ss)
{
	unsigned short crc;

    //printf("REC:%d\n",RxChar);
	if (RxChar == SSP_STX && ss->rxPtr == 0){
		// packet start
		ss->rxData[ss->rxPtr++] = RxChar;
	}else{
		// if last byte was start byte, and next is not then
		// restart the packet
		if (ss->CheckStuff == 1){
			if (RxChar != SSP_STX){
				ss->rxData[0] = SSP_STX;
				ss->rxData[1] = RxChar;
				ss->rxPtr = 2;
			}else
				ss->rxData[ss->rxPtr++] = RxChar;
			// reset stuff check flag
			ss->CheckStuff = 0;
		}else{
			// set flag for stuffed byte check
			if (RxChar == SSP_STX)
				ss->CheckStuff = 1;
			else{
				// add data to packet
				ss->rxData[ss->rxPtr++] = RxChar;
				// get the packet length
				if (ss->rxPtr == 	3)
					ss->rxBufferLength = ss->rxData[2] + 5;
			}
		}
		// are we at the end of the packet
		if (ss->rxPtr  == ss->rxBufferLength ){
			// is this packet for us ??
			if ((ss->rxData[1] & SSP_STX) == ss->SSPAddress){
				// is the checksum correct
				crc = cal_crc_loop_CCITT_A(ss->rxBufferLength - 3,&ss->rxData[1] ,CRC_SSP_SEED,CRC_SSP_POLY);
				if ((unsigned char)(crc & 0xFF) == ss->rxData[ss->rxBufferLength - 2] && (unsigned char)((crc >> 8) & 0xFF) == ss->rxData[ss->rxBufferLength - 1])
					ss->NewResponse = 1;  /* we have a new response so set flag  */
			}
			// reset packet
			ss->rxPtr  = 0;
			ss->CheckStuff = 0;
		}
	}


}
