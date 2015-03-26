#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <hiredis.h>
#include <async.h>
#include <adapters/libevent.h>

#include "main.h"
#include "ssp.h"

redisAsyncContext *db;
struct event sspPollEvent;
struct event_base *eventBase;

void cleanup(void) {
	if(db) {
		redisAsyncFree(db);
		db = NULL;
	}
	sspCleanup();
}

void terminate(void) {
	evtimer_del(&sspPollEvent);
	if(db) {
		redisAsyncCommand(db, NULL, NULL, "SET component:ssp 0");
		redisAsyncDisconnect(db);
		db = NULL;
	}
}

void interrupt(int signal) {
	terminate();
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

	// setup poll timer for validator
	struct timeval time;
	time.tv_sec = 0;
	time.tv_usec = 500000;

	event_set(&sspPollEvent, 0, EV_PERSIST, sspPoll, NULL);
	event_base_set(eventBase, &sspPollEvent);
	evtimer_add(&sspPollEvent, &time);

	redisAsyncCommand(db, NULL, NULL, "SET component:ssp 1");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
	if (status != REDIS_OK) {
		fprintf(stderr, "Database error: %s\n", c->errstr);
		return;
	}
	fprintf(stderr, "Disconnected from database\n");
}

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "Usage: %s /dev/<ssp-port>\n", argv[0]);
		return 1;
	}

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, interrupt);

	fprintf(stderr, "eSSP starting up (port %s)...\n", argv[1]);
	if(!sspConnectToValidator(argv[1])) {
		return 3;
	}
	eventBase = event_base_new();
	atexit(cleanup);
	setupDatabase();

	redisLibeventAttach(db, eventBase);
	redisAsyncSetConnectCallback(db, connectCallback);
	redisAsyncSetDisconnectCallback(db, disconnectCallback);

	event_base_dispatch(eventBase);

	return 0;
}
