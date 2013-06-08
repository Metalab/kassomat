#include "sspcoms.h"
#include <QDebug>

#include "sspcomstask.h"

#define CRC_SSP_SEED		0xFFFF
#define CRC_SSP_POLY		0x8005

SSPComs::SSPComs(const QSerialPortInfo &info) : m_port(NULL), m_portInfo(info), m_terminate(false), m_sequence(false) {
}

void SSPComs::startConnection() {
	if(!isRunning())
		start();
}

bool SSPComs::open() {
    m_port = new QSerialPort;
    m_port->setPort(m_portInfo);
    m_port->setBaudRate(QSerialPort::Baud9600);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::TwoStop);
    m_port->setFlowControl(QSerialPort::NoFlowControl);
	
    bool result = m_port->open(QIODevice::ReadWrite);
	if(!result)
		return false;
	
	
	
	return true;
}

SSPComs::~SSPComs() {
    m_port->close();
    delete m_port;
}

void SSPComs::run() {
	if(!open()) {
		qCritical() << "SSPComs: Failed opening serial port with error " << m_port->error() << ".";
		return;
	}
	
	QMutexLocker locker(&m_taskQueueMutex);
	while(!m_terminate) {
		while(!m_taskQueue.isEmpty()) {
			SSPComsTask *task = m_taskQueue.dequeue();
			// TODO: send request
			
			QByteArray response;
			// TODO: receive response
			QMetaObject::invokeMethod(task, "responseAvailable", Qt::QueuedConnection, Q_ARG(QByteArray, response));
			QMetaObject::invokeMethod(task, "deleteLater", Qt::QueuedConnection);
		}
		
		m_taskQueueUpdatedCondition.wait(&m_taskQueueMutex);
	}
	emit terminating();
}

///////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND ENCODING
///////////////////////////////////////////////////////////////////////////////////////////////

struct __attribute__((__packed__)) TransportLayer {
    uint8_t sequence:1;
    uint8_t slave_id:7;
    uint8_t length;
    uint8_t data_crc[];
};

uint16_t SSPComs::calculateCRC(const QByteArray &p, uint16_t seed, uint16_t cd) {
    int i, j;
    uint16_t crc = seed;

    for (i = 0; i < p.size(); ++i) {
        crc ^= ( p.constData()[i] << 8 );
        for(j = 0; j < 8; ++j) {
            if(crc & 0x8000)
                crc = (crc << 1) ^ cd;
            else
                crc <<= 1;
        }
    }
    return crc;
}


bool SSPComs::sendCommand(uint8_t slave_id, const QByteArray &cmd) {
    // see GA138 documentation page 9
    TransportLayer *transport = reinterpret_cast<TransportLayer *>(new uint8_t[sizeof(TransportLayer) + cmd.size() + 2]);

    transport->sequence = m_sequence;
    transport->slave_id = slave_id;
    transport->length = cmd.size();
    memcpy(transport->data_crc, cmd.constData(), cmd.size());

    // Add CRC

    uint16_t crc = calculateCRC(QByteArray(reinterpret_cast<const char*>(transport), sizeof(TransportLayer) + cmd.size()), CRC_SSP_SEED, CRC_SSP_POLY);
    transport->data_crc[cmd.size()] = crc & 0xFF;
    transport->data_crc[cmd.size()+1] = crc >> 8;

    // Next up: stuffing

    QByteArray transportBytes(reinterpret_cast<const char*>(transport), sizeof(TransportLayer) + cmd.size() + 2);
    uint8_t stuffing = transportBytes.count(0x7f);

    QByteArray stuffedTransportBytes(1 + transportBytes.size() + stuffing, 0x7f);
    int stuffedIndex = 1; // first byte is stx (0x4f)

    for(int i = 0; i < transportBytes.size(); ++i) {
        uint8_t byte = transportBytes.data()[i];
        if(byte == 0x7f) {
            stuffedIndex += 2; // bytes default to 0x4f anyways
        } else {
            stuffedTransportBytes.data()[stuffedIndex++] = byte;
        }
    }

    qint64 bytesWritten = m_port->write(stuffedTransportBytes);

    if(bytesWritten != stuffedTransportBytes.size()) {
        if(bytesWritten == -1) {
            qCritical() << "SSPComs::SSPSendCommand: Writing to kassomat failed!";
        } else {
            qWarning() << "SSPComs::SSPSendCommand: Didn't write all bytes!";
        }
        delete [] reinterpret_cast<uint8_t*>(transport);
        return false;
    }

    m_sequence = !m_sequence;

	delete [] reinterpret_cast<uint8_t*>(transport);
    return true;
}

QByteArray SSPComs::encrypt(const QByteArray &cmd) {
    // see GA138 documentation page 11
    QByteArray encryptedData(1, 0x7e);



    return encryptedData;
}

void SSPComs::SSPResponseAvailable(int socket) {
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND GENERATION
///////////////////////////////////////////////////////////////////////////////////////////////

SSPComs::Result SSPComs::reset() {

}

void SSPComs::disable() {

}

void SSPComs::datasetVersion(std::function<void(const QString&)> callback) {
	m_taskQueueMutex.lock();
	m_taskQueue.enqueue(new SSPComsTask(QByteArray(1, 0x21), [callback](const QByteArray &response) {
		callback(QString::fromUtf8(response));
	}));
	m_taskQueueUpdatedCondition.wakeOne();
	m_taskQueueMutex.unlock();
}

SSPComs::Result_Payout SSPComs::payout(uint32_t amount, bool test) {

}

uint32_t SSPComs::getDenominationLevel(uint32_t denomination, const QString &currency) {

}

SSPComs::Result_Route SSPComs::getDenominationRoute(uint32_t denomination, const QString &currency) {

}

uint32_t SSPComs::getMinimalPayout(const QString &currency) {

}

void SSPComs::setCoinInhibits(bool inhibited, uint32_t denomination, const QString &currency) {

}

SSPComs::Result_Payout SSPComs::payoutByDenomination(const QList<SSPComs::PayoutDenomination> &requests) {

}

void SSPComs::smartEmpty() {

}

void SSPComs::poll() {

}

uint32_t SSPComs::getSerialNumber() {

}

QString SSPComs::getFirmwareVersion() {

}

const QList<SSPComs::PayoutDenomination> SSPComs::getAllLevels() {

}

bool SSPComs::setDenominationLevel(uint32_t numToAdd, uint32_t denomination, const QString &currency) {

}

void SSPComs::haltPayout() {

}

SSPComs::Result_Payout SSPComs::floatAmount(uint32_t minPayout, uint32_t totalAmount, const QString &currency, bool test) {

}

void SSPComs::emptyAll() {

}

void SSPComs::setCoinMechGlobalInhibit(bool enable) {

}

QList<SSPComs::PayoutDenomination> SSPComs::cashboxPayoutOperationData() { // after smartEmpty, emptyAll, floatAmount, "dispense"

}
