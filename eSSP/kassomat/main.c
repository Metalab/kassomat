#include <stdio.h>
#include <SSPComs.h>
#include <hiredis.h>

#include <stdlib.h>

int main(int argc, char **argv) {
	fprintf(stderr, "eSSP starting up...\n");

	SSP_COMMAND sspC;

	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	redisContext *db = redisConnectWithTimeout("127.0.0.1", 6379, timeout);

	if(db == NULL || db->err) {
		if(db) {
			fprintf(stderr, "Connection error: %s\n", db->errstr);
			redisFree(db);
		} else {
			fprintf(stderr, "Connection error: can't allocate redis context\n");
		}
		exit(1);
	}
	redisEnableKeepAlive(db);

	redisReply *reply;

	reply = redisCommand(db,"PING");
	printf("PING: %s\n", reply->str);
	freeReplyObject(reply);


	redisFree(db);

	return 0;
}
