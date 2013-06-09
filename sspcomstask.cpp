#include "sspcomstask.h"

void SSPComsTask::responseAvailable(QByteArray data) {
	responder(data[0], data.right(data.length()-1));
}
