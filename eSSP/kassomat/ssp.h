#ifndef __SSP_H
#define __SSP_H

#include <stdbool.h>

bool sspConnectToValidator(const char * const device);
void sspPoll(int fd, short event, void *arg);

void sspCleanup(void);

#endif
