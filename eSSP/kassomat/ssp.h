#ifndef __SSP_H
#define __SSP_H

#include <stdbool.h>

struct SSPDenomination {
	unsigned long value;
	unsigned long count;
};

typedef enum {
	sspResultOk = 0xF0,
	sspResultUnknownCommand = 0xF2,
	sspResultIncorrectParameters = 0xF3,
	sspResultInvalidParameter = 0xF4,
	sspResultCommandNotProcessed = 0xF5,
	sspResultSoftwareError = 0xF6,
	sspResultChecksumError = 0xF7,
	sspResultFailure = 0xF8,
	sspResultHeaderFailure = 0xF9,
	sspResultKeyNotSet = 0xFA,
	sspResultTimeout = 0xFF
} sspResult;

bool sspConnectToValidator(const char * const device);
void sspPoll(int fd, short event, void *arg);

sspResult sspEmpty(void);
sspResult sspGetAllLevels(void);
sspResult sspPayout(int value);
sspResult sspPayoutByDenomination(unsigned count, const struct SSPDenomination * const denominationList, bool test);
sspResult sspSetBezel(unsigned char r, unsigned char g, unsigned char b);

void sspCleanup(void);

#endif
