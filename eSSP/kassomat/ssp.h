#ifndef __SSP_H
#define __SSP_H

#include <stdbool.h>

struct SSPDenomination {
	uint32_t value;
	uint16_t count;
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

typedef enum {
	sspPayoutNotEnoughValue = 0,
	sspPayoutCannotPayExact = 1,
	sspPayoutDeviceBusy = 3,
	sspPayoutDeviceDisabled = 4
} sspPayoutResult;

bool sspConnectToValidator(const char * const device);
void sspPoll(int fd, short event, void *arg);

sspResult sspEmpty(void);
sspResult sspGetAllLevels(uint8_t *count, struct SSPDenomination **levels); // *levels must NOT be freed!
sspResult sspPayout(uint32_t value, bool test, sspPayoutResult *error); // error only set when result == sspResultCommandNotProcessed
sspResult sspPayoutByDenomination(uint8_t count, const struct SSPDenomination * const denominationList, bool test, sspPayoutResult *error); // see above
sspResult sspConfigureBezel(uint8_t r, uint8_t g, uint8_t b, bool store);

void sspCleanup(void);

#endif
