#include "sspcoms.h"
#include <QDebug>

#include "sspcomstask.h"
#include "sspevents.h"

#include <errno.h>
#include <random>
#include <openssl/bn.h>

#define CRC_SSP_SEED		0xFFFF
#define CRC_SSP_POLY		0x8005

SSPComs::SSPComs(const QSerialPortInfo &info) : m_port(NULL), m_portInfo(info), m_terminate(false), m_sequence(false), m_encryptionEnabled(false), m_key(16,0) {
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

void SSPComs::setTerminate(bool terminate) {
	m_terminate = terminate;
	m_taskQueueUpdatedCondition.wakeAll();
}

void SSPComs::run() {
	if(!open()) {
		qCritical() << "SSPComs: Failed opening serial port with error " << m_port->error() << ".";
		qCritical() << "errno = " << errno;
		return;
	}

    negotiateEncryption(0x123456701234567ULL);

	QMutexLocker locker(&m_taskQueueMutex);
	while(!m_terminate) {
		while(!m_taskQueue.isEmpty()) {
			SSPComsTask *task = m_taskQueue.dequeue();
			if(!sendCommand(0x00, task->message())) {
				// OMG OMG OMG
			}
			
			QByteArray response = readResponse();
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

QByteArray SSPComs::readResponse() {
	// TODO: verify sequence and slave id
	QByteArray recv;
	while(true) {
		m_port->waitForReadyRead(1000000);
		recv.append(m_port->readAll());
		
		if(recv.length() >= 5) {
			if(recv.constData()[0] != 0x7f) {
				qCritical() << "Read response that doesn't start with STX. (got " << (int)*recv.constData() << ")";
				throw QString("Read response that doesn't start with STX.");
			}
			const TransportLayer *transport = reinterpret_cast<const TransportLayer *>(&recv.constData()[1]);
			uint8_t length = transport->length;
			QByteArray content;
			uint8_t recvIndex = 3;
			for(uint8_t i = 0; i < length; ++i) {
				if(recv.length() < recvIndex+1) {
					// not enough data, try to capture more
					continue;
				}
				char date = recv[recvIndex++];
				content.append(date);
				if(date == 0x7f) {
					if(recv.length() < recvIndex+1) {
						// not enough data, try to capture more
						continue;
					}
					if(recv[recvIndex++] != char(0x7f)) {
						qCritical() << "Non-stuffed byte received!";
						throw "Non-stuffed byte received!";
					}
				}
			}
			if(recv.length() < recvIndex + 2) {
				// not enough data, try to capture more
				continue;
			}
			uint8_t crc1 = recv[recvIndex++];
			if(crc1 == 0x7f) {
				if(recv.length() < recvIndex + 2) {
					// not enough data, try to capture more
					continue;
				}
				crc1 = recv[recvIndex++];
				if(crc1 != 0x7f) {
					qCritical() << "Non-stuffed byte received!";
					throw "Non-stuffed byte received!";
				}
			}
			if(recv.length() < recvIndex + 1) {
				// not enough data, try to capture more
				continue;
			}
			uint8_t crc2 = recv[recvIndex++];
			if(crc2 == 0x7f) {
				if(recv.length() < recvIndex + 1) {
					// not enough data, try to capture more
					continue;
				}
				crc2 = recv[recvIndex++];
				if(crc2 != 0x7f) {
					qCritical() << "Non-stuffed byte received!";
					throw "Non-stuffed byte received!";
				}
			}
			
			uint16_t crc = (crc2 << 8) | crc1;
			// verify CRC
			if(crc != calculateCRC(recv.mid(1, sizeof(TransportLayer)) + content, CRC_SSP_SEED, CRC_SSP_POLY)) {
				qCritical() << "Bad CRC";
				throw QString("Bad CRC");
			}
			
			return content;
		}
	}
}

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


bool SSPComs::sendCommand(uint8_t slave_id, const QByteArray &cmd_plain) {
    // see GA138 documentation page 9
    QByteArray cmd = encrypt(cmd_plain);
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
    int stuffedIndex = 1; // first byte is stx (0x7f)

    for(int i = 0; i < transportBytes.size(); ++i) {
        uint8_t byte = transportBytes.data()[i];
        if(byte == 0x7f) {
            stuffedIndex += 2; // bytes default to 0x7f anyways
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

struct BIGNUMPtr {
    BIGNUM *val;
    BIGNUMPtr() {
        val = BN_new();
    }
    ~BIGNUMPtr() {
        BN_free(val);
    }
    BIGNUM *operator*() {
        return val;
    }
    const BIGNUM *operator*() const {
        return val;
    }
};

void SSPComs::negotiateEncryption(uint64_t fixedKey) {
    // Diffie-Hellman keyexchange
    BIGNUMPtr generator, modulus, hostInterKey, hostRandom;

    std::random_device rd;
    uniform_int_distribution<uint8_t> dist;
    uchar rnd_seed[16];
    for(int i=0; i < 16; ++i)
        rnd_seed[i] = dist(rd);

    RAND_seed(rnd_seed, sizeof rnd_seed);

    BN_generate_prime_ex(*generator, 64, 0, NULL, NULL, NULL);
    BN_generate_prime_ex(*modulus, 64, 0, NULL, NULL, NULL);

    // Doesn't make sense when the generator is larger than the modulus value
    if(BN_cmp(*generator, *modulus) == 1)
        std::swap(*generator, *modulus);

    BN_rand(*hostRandom, 64, -1, 0);

    // hostInterKey = generator ^ hostRandom % modulus
    BN_mod_exp(*hostInterKey, *generator, *hostRandom, *modulus);

    QByteArray generatorData(BN_num_bytes(*generator), 0);
    BN_bn2bin(*generator, generatorData.data());

    QByteArray setGenerator(9, 0);
    setGenerator[0] = 0x4a;
    std::reverse_copy(generatorData.constBegin(), generatorData.constEnd(), setGenerator.begin()+1);

    if(!sendCommand(0, setGenerator)) {
        throw "OMG OMG OMG";
    }

    QByteArray response = readResponse();
    if(response[0] != 0xf0) {
        throw "Failed sending Set Generator command";
    }

    QByteArray modulusData(BN_num_bytes(*modulus), 0);
    BN_bn2bin(*modulus, modulusData.data());

    QByteArray setModulus(9, 0);
    setModulus[0] = 0x4b;
    std::reverse_copy(modulusData.constBegin(), modulusData.constEnd(), setModulus.begin()+1);

    if(!sendCommand(0, setModulus)) {
        throw "OMG OMG OMG";
    }

    response = readResponse();
    if(response[0] != 0xf0) {
        throw "Failed sending Set Modulus command";
    }

    QByteArray hostInterKeyData(BN_num_bytes(*hostInterKey), 0);
    BN_bn2bin(*hostInterKey, hostInterKeyData.data());

    QByteArray requestKeyExchange(9, 0);
    requestKeyExchange[0] = 0x4c;

    std::reverse_copy(hostInterKey->constBegin(), hostInterKey->constEnd(), requestKeyExchange.begin()+1);

    if(!sendCommand(0, requestKeyExchange)) {
        throw "OMG OMG OMG";
    }

    response = readResponse();
    if(response[0] != 0xf0 || response.length() != 9) {
        throw "Failed sending Request Key Exchange command";
    }

    // DOCUMENTATION BUG: Request Key Exchange returns the SlaveInterKey!

    QByteArray slaveInterKeyData(8, 0);
    std::reverse_copy(response.constBegin()+1, response.constEnd(), slaveInterKeyData.begin());

    BIGNUMPtr slaveInterKey, secretKey;
    BN_bin2bn(slaveInterKeyData.data(), slaveInterKeyData.length(), *slaveInterKey);

    // key = slaveInterKey ^ hostRandom % modulus
    BN_mod_exp(*secretKey, *slaveInterKey, *hostRandom, *modulus);

    QByteArray secretKeyData(BN_num_bytes(*secretKey), 0);
    BN_bn2bin(*secretKey, secretKeyData.data());

    m_key.fill(0);
    m_key.replace(0, 8, static_cast<const char*>(&fixedKey), 8);
    std::reverse_copy(secretKeyData.constBegin(), secretKeyData.constEnd(), m_key.begin() += 8);
    m_encryptionEnabled = true;
    m_encryptionCount = 0;
}

QByteArray SSPComs::encrypt(const QByteArray &cmd) {
    if(!m_encryptionEnabled)
        return cmd;
    // see GA138 documentation page 11
    AES_KEY enc_key;
    AES_set_encrypt_key(m_key.constData(), 128, &enc_key);

    // packet length + packet count
    QByteArray bytesToEncrypt;
    bytesToEncrypt.append(static_cast<uint8_t>(cmd.length()));
    bytesToEncrypt.append(m_encryptionCount);
    for(uint8_t i = 0; i < 4; i++) // byte ordering!
        bytesToEncrypt.append(static_cast<uint8_t>(m_encryptionCount >> (8*i) & 0xFF));

    // padding
    uint8_t packLength = 16 - ((cmd.length()+7) % 16);
    if(packLength > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        uniform_int_distribution<uint8_t> dist;

        for(uint8_t i = 0; i < packLength; i++)
            bytesToEncrypt.append(dist(gen));
    }

    // crc
    uint16_t crc = calculateCRC(bytesToEncrypt, CRC_SSP_SEED, CRC_SSP_POLY);
    bytesToEncrypt.append(static_cast<uint8_t>(crc & 0xFF));
    bytesToEncrypt.append(static_cast<uint8_t>(crc >> 8));

    QByteArray encryptedData(1 + bytesToEncrypt.length(), 0x7e); // STEX

    // encrypt in 16 byte blocks
    for(uint8_t i = 0; i < bytesToEncrypt.length() / 16; i++)
        AES_encrypt(bytesToEncrypt.constData() + i * 16, encryptedData.data() + (1 + i * 16), &enc_key);

    // increment packet counter after the packet was encrypted
    m_encryptionCount++;

    return encryptedData;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// COMMAND GENERATION
///////////////////////////////////////////////////////////////////////////////////////////////

void SSPComs::enqueueTask(const QByteArray &data, const std::function<void(uint8_t, const QByteArray&)> &response) {
	QMutexLocker locker(&m_taskQueueMutex);
	m_taskQueue.enqueue(new SSPComsTask(data, response));
	m_taskQueueUpdatedCondition.wakeOne();
}

void SSPComs::reset(std::function<void()> callback) {
	enqueueTask(QByteArray(1, 0x01), [callback](uint8_t, const QByteArray &response) {
		callback();
    });
}

void SSPComs::disable(std::function<void()> callback) {
	enqueueTask(QByteArray(1, 0x09), [callback](uint8_t, const QByteArray &response) {
		callback();
    });
}

void SSPComs::datasetVersion(std::function<void(const QString&)> callback) {
	enqueueTask(QByteArray(1, 0x21), [callback](uint8_t, const QByteArray &response) {
		callback(QString::fromUtf8(response));
    });
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

void SSPComs::poll(std::function<void(QList<SSPEvent*>)> callback) {
	enqueueTask(QByteArray(1, 0x7), [callback](uint8_t, const QByteArray &response) {
		QList<SSPEvent*> events;
		
		// TODO parse events
		
		callback(events);
    });
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
