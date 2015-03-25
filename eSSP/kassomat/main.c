#include <stdlib.h>
#include <stdio.h>
#include <SSPComs.h>
#include <signal.h>
#include <hiredis.h>
#include <async.h>
#include <adapters/libevent.h>

void connectToValidator(SSP_COMMAND *sspC);
void sspPoll(int fd, short event, void *arg);

redisAsyncContext *db;
struct event sspPollEvent;
struct event_base *eventBase;

char const *sspAddress;
int sspPort;

void cleanup(void) {
	if(db) {
		redisAsyncFree(db);
	}
	CloseSSPPort(sspPort);
}

void interrupt(int signal) {
	evtimer_del(&sspPollEvent);
	redisAsyncDisconnect(db);
	db = NULL;
}

void setupDatabase(void) {
	db = redisAsyncConnect("127.0.0.1", 6379);

	if(db == NULL || db->err) {
		if(db) {
			fprintf(stderr, "Connection error: %s\n", db->errstr);
		} else {
			fprintf(stderr, "Connection error: can't allocate redis context\n");
		}
		exit(1);
	}
}

void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        fprintf(stderr, "Database error: %s\n", c->errstr);
        return;
    }
    fprintf(stderr, "Connected to database...\n");

		SSP_COMMAND sspC;
		sspC.Timeout = 1000;
	  sspC.EncryptionStatus = NO_ENCRYPTION;
	  sspC.RetryLevel = 3;
	  sspC.BaudRate = 9600;
		connectToValidator(&sspC);
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        fprintf(stderr, "Database error: %s\n", c->errstr);
        return;
    }
    fprintf(stderr, "Disconnected from database\n");
}

void connectToValidator(SSP_COMMAND *sspC) {
	fprintf(stderr, "Connecting to validator...\n");

	struct timeval time;
	time.tv_sec = 0;
	time.tv_usec = 500000;

	event_set(&sspPollEvent, 0, EV_PERSIST, sspPoll, NULL);
	event_base_set(eventBase, &sspPollEvent);
	evtimer_add(&sspPollEvent, &time);
}

void sspPoll(int fd, short event, void *arg) {
	fprintf(stderr, "Poll\n");
}

int send_ssp_command(SSP_COMMAND *sspC) {
	return SSPSendCommand(sspPort, sspC);
}

int negotiate_ssp_encryption(SSP_COMMAND *sspC, SSP_FULL_KEY * hostKey){
	return NegotiateSSPEncryption(sspPort, sspC->SSPAddress, hostKey);
}

int main(int argc, char **argv) {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, interrupt);
	eventBase = event_base_new();

	fprintf(stderr, "eSSP starting up...\n");
	atexit(cleanup);
	setupDatabase();

	redisLibeventAttach(db, eventBase);
	redisAsyncSetConnectCallback(db, connectCallback);
	redisAsyncSetDisconnectCallback(db, disconnectCallback);

	event_base_dispatch(eventBase);

	return 0;
}
