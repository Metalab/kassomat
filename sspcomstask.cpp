#include "sspcomstask.h"

void SSPComsTask::responseAvailable(QByteArray data) {
	responder(data);
}
