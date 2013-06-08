#include "sspcomstask.h"

void SSPComsTask::responseAvailable(const QByteArray &data) {
	responder(data);
}
