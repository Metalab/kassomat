#include "sspcoms.h"
#include <QDebug>

#include "sspcomstask.h"
#include "sspevents.h"

#include <errno.h>
#include <random>
#include <openssl/bn.h>
#include <openssl/rand.h>

#define CRC_SSP_SEED		0xFFFF
#define CRC_SSP_POLY		0x8005

QString toDebug(const QByteArray & line) {
    QString s;
    uchar c;

    for ( int i=0 ; i < line.size() ; i++ ){
        c = line[i];
        s.append(QString("%1").arg(c, 2, 16, QLatin1Char('0')));
        if(i+1 < line.size()) {
            s.append(' ');
        }
    }
    return s;
}

SSPComs::SSPComs(const QSerialPortInfo &info) : m_port(NULL), m_portInfo(info), m_terminate(false), m_sequence(false), m_encryptionEnabled(false), m_key(16,0) {
    m_bn_ctx = BN_CTX_new();
}

void SSPComs::startConnection() {
	if(!isRunning())
		start();
}

bool SSPComs::open() {
    m_port = new QSerialPort;
    m_port->setPort(m_portInfo);
	
    bool result = m_port->open(QIODevice::ReadWrite);
    if(!result) {
        qCritical() << "SSPComs::open: could not open port";
		return false;
    }

    m_port->setBaudRate(QSerialPort::Baud9600);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::TwoStop);
    m_port->setFlowControl(QSerialPort::NoFlowControl);

	return true;
}

SSPComs::~SSPComs() {
    m_port->close();
    delete m_port;
    BN_CTX_free(m_bn_ctx);
}

void SSPComs::setTerminate(bool terminate) {
	m_terminate = terminate;
	m_taskQueueUpdatedCondition.wakeAll();
}

void SSPComs::run() {
	if(!open()) {
        qCritical() << "SSPComs: Failed opening serial port with error" << m_port->error();
        qCritical() << "Error" << errno << ":" << strerror(errno);
		return;
	}

    if(sync())
        m_sequence = false;

    negotiateEncryption(0x123456701234567ULL);

	QMutexLocker locker(&m_taskQueueMutex);
	while(!m_terminate) {
		while(!m_taskQueue.isEmpty()) {
			SSPComsTask *task = m_taskQueue.dequeue();
            bool retry = false;
            QByteArray response;
            do {
                if(!sendCommand(0x00, task->message(), retry)) {
                    // OMG OMG OMG
                }

                retry = false;
                try {
                    response = readResponse();
                } catch(TimeoutException e) {
                    qDebug() << "caught timeout";
                    retry = true;
                }
            } while(retry);
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
    uint8_t slave_id:7;
    uint8_t sequence:1;
    uint8_t length;
    uint8_t data_crc[];
};

QByteArray SSPComs::readResponse() {
	// TODO: verify sequence and slave id
	QByteArray recv;
	while(true) {
        if(!m_port->waitForReadyRead(1000)) {
            throw TimeoutException();
        }
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

            qDebug() << "Reading Bytes:" << toDebug(recv);
			
			uint16_t crc = (crc2 << 8) | crc1;
			// verify CRC
			if(crc != calculateCRC(recv.mid(1, sizeof(TransportLayer)) + content, CRC_SSP_SEED, CRC_SSP_POLY)) {
				qCritical() << "Bad CRC";
				throw QString("Bad CRC");
			}

            QByteArray message = decrypt(content);
            qDebug() << "Received message:" << toDebug(message);

            return message;
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


bool SSPComs::sendCommand(uint8_t slave_id, const QByteArray &cmd_plain, bool retry) {
    // see GA138 documentation page 9
    QByteArray cmd = encrypt(cmd_plain, retry);
    TransportLayer *transport = reinterpret_cast<TransportLayer *>(new uint8_t[sizeof(TransportLayer) + cmd.size() + 2]);

    if(retry)
        m_sequence = !m_sequence;
    if(cmd_plain[0] == 0x11) // reset for sync command
        m_sequence = true;

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

    qDebug() << "Sending Bytes:" << toDebug(stuffedTransportBytes);

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
    bool retry = false;
    QByteArray response;

    BIGNUMPtr generator, modulus, hostInterKey, hostRandom;

    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist;
    uchar rnd_seed[16];
    for(int i=0; i < 16; ++i)
        rnd_seed[i] = dist(rd);

    RAND_seed(rnd_seed, sizeof rnd_seed);

    // FIRMWARE BUG: Everything beyond 32bit fails!
    // According to the documentation, 64bits should be possible.
    BN_generate_prime_ex(*generator, 32, 0, NULL, NULL, NULL);
    BN_generate_prime_ex(*modulus, 32, 0, NULL, NULL, NULL);

    // Doesn't make sense when the generator is larger than the modulus value
    if(BN_cmp(*generator, *modulus) == 1)
        BN_swap(*generator, *modulus);

    BN_rand(*hostRandom, 64, -1, 0);

    // hostInterKey = generator ^ hostRandom % modulus
    BN_mod_exp(*hostInterKey, *generator, *hostRandom, *modulus, m_bn_ctx);

    QByteArray generatorData(BN_num_bytes(*generator), 0);
    BN_bn2bin(*generator, reinterpret_cast<uint8_t*>(generatorData.data()));

    QByteArray setGenerator(9, 0);
    setGenerator[0] = 0x4a;
    std::reverse_copy(generatorData.constBegin(), generatorData.constEnd(), setGenerator.begin()+1);

    do {
        if(!sendCommand(0, setGenerator, retry)) {
            qCritical() << "setGenerator failed!";
            throw "OMG OMG OMG";
        }
        retry = false;
        try {
            response = readResponse();
        } catch(TimeoutException e) {
            retry = true;
        }
    } while(retry);

    if(static_cast<uint8_t>(response[0]) != 0xf0) {
        qCritical() << "Failed sending Set Generator command";
        throw "Failed sending Set Generator command";
    }

    QByteArray modulusData(BN_num_bytes(*modulus), 0);
    BN_bn2bin(*modulus, reinterpret_cast<uint8_t*>(modulusData.data()));

    QByteArray setModulus(9, 0);
    setModulus[0] = 0x4b;
    std::reverse_copy(modulusData.constBegin(), modulusData.constEnd(), setModulus.begin()+1);

    if(!sendCommand(0, setModulus)) {
        qCritical() << "setModulus failed!";
        throw "OMG OMG OMG";
    }

    response = readResponse();
    if(static_cast<uint8_t>(response[0]) != 0xf0) {
        qCritical() << "Failed sending Set Modulus command";
        throw "Failed sending Set Modulus command";
    }

    QByteArray hostInterKeyData(BN_num_bytes(*hostInterKey), 0);
    BN_bn2bin(*hostInterKey, reinterpret_cast<uint8_t*>(hostInterKeyData.data()));

    QByteArray requestKeyExchange(9, 0);
    requestKeyExchange[0] = 0x4c;

    std::reverse_copy(hostInterKeyData.constBegin(), hostInterKeyData.constEnd(), requestKeyExchange.begin()+1);

    if(!sendCommand(0, requestKeyExchange)) {
        qCritical() << "requestKeyExchange failed!";
        throw "OMG OMG OMG";
    }

    response = readResponse();
    if(static_cast<uint8_t>(response[0]) != 0xf0 || response.length() != 9) {
        qCritical() << "Failed sending Request Key Exchange command";
        throw "Failed sending Request Key Exchange command";
    }

    // DOCUMENTATION BUG: Request Key Exchange returns the SlaveInterKey!

    QByteArray slaveInterKeyData(8, 0);
    std::reverse_copy(response.constBegin()+1, response.constEnd(), slaveInterKeyData.begin());

    BIGNUMPtr slaveInterKey, secretKey;
    BN_bin2bn(reinterpret_cast<uint8_t*>(slaveInterKeyData.data()), slaveInterKeyData.length(), *slaveInterKey);

    // key = slaveInterKey ^ hostRandom % modulus
    BN_mod_exp(*secretKey, *slaveInterKey, *hostRandom, *modulus, m_bn_ctx);

    QByteArray secretKeyData(BN_num_bytes(*secretKey), 0);
    BN_bn2bin(*secretKey, reinterpret_cast<uint8_t*>(secretKeyData.data()));

    m_key.fill(0);
    m_key.replace(0, 8, reinterpret_cast<const char*>(&fixedKey), 8);
    std::reverse_copy(secretKeyData.constBegin(), secretKeyData.constEnd(), m_key.begin() + 8);
    m_encryptionEnabled = true;
    m_encryptionCount = 0;
}

QByteArray SSPComs::encrypt(const QByteArray &cmd, bool retry) {
    if(!m_encryptionEnabled)
        return cmd;
    // see GA138 documentation page 11
    AES_KEY enc_key;
    AES_set_encrypt_key(reinterpret_cast<const uint8_t*>(m_key.constData()), 128, &enc_key);

    // packet length + packet count
    QByteArray bytesToEncrypt;
    bytesToEncrypt.append(static_cast<uint8_t>(cmd.length()));
    //bytesToEncrypt.append(m_encryptionCount);
    for(uint8_t i = 0; i < 4; i++) // byte ordering!
        bytesToEncrypt.append(static_cast<uint8_t>(m_encryptionCount >> (8*i) & 0xFF));

    bytesToEncrypt.append(cmd);

    // padding
    uint8_t packLength = 16 - ((cmd.length()+7) % 16);
    if(packLength > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint8_t> dist;

        for(uint8_t i = 0; i < packLength; i++)
            bytesToEncrypt.append(dist(gen));
    }

    // crc
    uint16_t crc = calculateCRC(bytesToEncrypt, CRC_SSP_SEED, CRC_SSP_POLY);
    bytesToEncrypt.append(static_cast<uint8_t>(crc & 0xFF));
    bytesToEncrypt.append(static_cast<uint8_t>(crc >> 8));

    QByteArray encryptedData(1 + bytesToEncrypt.length(), 0x7e); // STEX

    qDebug() << "Encrypting data:" << toDebug(bytesToEncrypt);

    // encrypt in 16 byte blocks
    for(uint8_t i = 0; i < bytesToEncrypt.length() / 16; i++)
        AES_encrypt(reinterpret_cast<const uint8_t*>(bytesToEncrypt.constData()) + i * 16, reinterpret_cast<uint8_t*>(encryptedData.data()) + (1 + i * 16), &enc_key);

    return encryptedData;
}

QByteArray SSPComs::decrypt(const QByteArray &cmd) {
    if(!m_encryptionEnabled || cmd[0] != 0x7e)
        return cmd;
    AES_KEY dec_key;
    AES_set_decrypt_key(reinterpret_cast<const uint8_t*>(m_key.constData()), 128, &dec_key);

    QByteArray decryptedData(cmd.length()-1,0);

    for(uint8_t i = 0; i < (cmd.length()-1) / 16; i++)
        AES_decrypt(reinterpret_cast<const uint8_t*>(cmd.constData()) + 1+i*16, reinterpret_cast<uint8_t*>(decryptedData.data()) + i*16, &dec_key);

    uint8_t length = decryptedData[0];
    if(length > decryptedData.length() - 7) {
        qCritical() << "Length in encrypted packet is invalid";
        throw "Length in encrypted packet is invalid";
    }
    uint16_t count = decryptedData[1] | (decryptedData[2] << 8) | (decryptedData[3] << 16) | (decryptedData[4] << 24);
    m_encryptionCount++;
    if(count != m_encryptionCount) {
        qCritical() << "Encryption Counter of received packet is incorrect";
        throw "Encryption Counter of received packet is incorrect";
    }

    qDebug() << "Decrypted data:" << toDebug(decryptedData);

    uint16_t crc = ((uint8_t)decryptedData[decryptedData.length()-2]) | (((uint8_t)decryptedData[decryptedData.length()-1]) << 8);

    if(crc != calculateCRC(decryptedData.left(decryptedData.length()-2), CRC_SSP_SEED, CRC_SSP_POLY)) {
        qCritical() << "Bad CRC";
        throw "Bad CRC";
    }

    return decryptedData.mid(5, length);
}

bool SSPComs::sync() {
    bool retry = false;
    QByteArray response;
    do {
        if(!sendCommand(0, QByteArray(1, 0x11), retry)) {
            qCritical() << "sync failed";
            throw "OMG OMG OMG";
        }
        retry = false;
        try {
            response = readResponse();
        } catch(TimeoutException e) {
            retry = true;
        }
    } while(retry);
    return static_cast<uint8_t>(response[0]) == 0xf0;
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

void SSPComs::enable(std::function<void()> callback) {
    enqueueTask(QByteArray(1, 0x0A), [callback](uint8_t, const QByteArray &response) {
        callback();
    });
}

void SSPComs::emptyAll(std::function<void()> callback) {
    enqueueTask(QByteArray(1, 0x3F), [callback](uint8_t, const QByteArray &response) {
        callback();
    });
}

void SSPComs::displayOn(std::function<void()> callback) {
    enqueueTask(QByteArray(1, 0x3), [callback](uint8_t, const QByteArray &response) {
        callback();
    });
}

void SSPComs::displayOff(std::function<void()> callback) {
    enqueueTask(QByteArray(1, 0x4), [callback](uint8_t, const QByteArray &response) {
        callback();
    });
}

void SSPComs::datasetVersion(std::function<void(const QString&)> callback) {
    enqueueTask(QByteArray(1, 0x21), [callback](uint8_t, const QByteArray &response) {
        callback(QString::fromUtf8(response));
    });
}

void SSPComs::firmwareVersion(std::function<void(const QString&)> callback) {
    enqueueTask(QByteArray(1, 0x20), [callback](uint8_t, const QByteArray &response) {
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

void SSPComs::poll(std::function<void(QList<QSharedPointer<SSPEvent>>)> callback) {
	enqueueTask(QByteArray(1, 0x7), [callback](uint8_t, const QByteArray &response) {
        QList<QSharedPointer<SSPEvent>> events;
		
        QByteArray remainingResponse(response);
        while(remainingResponse.length() > 0) {
            QSharedPointer<SSPEvent> event;

            unsigned char eventId = remainingResponse[0];
            qDebug() << "poll: parsing eventId:" << hex << eventId;

            switch(eventId) {
            case 0xf1:
                event.reset(new SSPResetEvent(remainingResponse));
                break;
            case 0xef:
                event.reset(new SSPReadNoteEvent(remainingResponse));
                break;
            case 0xee:
                event.reset(new SSPCreditNoteEvent(remainingResponse));
                break;
            case 0xed:
                event.reset(new SSPNoteRejectingEvent(remainingResponse));
                break;
            case 0xec:
                event.reset(new SSPNoteRejectedEvent(remainingResponse));
                break;
            case 0xcc:
                event.reset(new SSPNoteStackingEvent(remainingResponse));
                break;
            case 0xeb:
                event.reset(new SSPNoteStackedEvent(remainingResponse));
                break;
            case 0xea:
                event.reset(new SSPSafeNoteJamEvent(remainingResponse));
                break;
            case 0xe9:
                event.reset(new SSPUnsafeNoteJamEvent(remainingResponse));
                break;
            case 0xe8:
                event.reset(new SSPDisabledEvent(remainingResponse));
                break;
            case 0xe6:
                event.reset(new SSPFraudAttemptEvent(remainingResponse));
                break;
            case 0xe7:
                event.reset(new SSPStackerFullEvent(remainingResponse));
                break;
            case 0xe1:
                event.reset(new SSPNoteClearedFromFrontEvent(remainingResponse));
                break;
            case 0xe2:
                event.reset(new SSPNoteClearedToCashboxEvent(remainingResponse));
                break;
            case 0xe3:
                event.reset(new SSPCashboxRemovedEvent(remainingResponse));
                break;
            case 0xe4:
                event.reset(new SSPCashboxReplacedEvent(remainingResponse));
                break;
            case 0xe5:
                event.reset(new SSPBarCodeTicketValidatedEvent(remainingResponse));
                break;
            case 0xd1:
                event.reset(new SSPBarCodeTicketAcknowledgeEvent(remainingResponse));
                break;
            case 0xe0:
                event.reset(new SSPNotePathOpenEvent(remainingResponse));
                break;
            case 0xb5:
                event.reset(new SSPChannelDisableEvent(remainingResponse));
                break;
            case 0xb6:
                event.reset(new SSPInitiatingEvent(remainingResponse));
                break;
            case 0xda:
                event.reset(new SSPDispensingEvent(remainingResponse));
                break;
            case 0xd2:
                event.reset(new SSPDispensedEvent(remainingResponse));
                break;
            case 0xd5:
                event.reset(new SSPJammedEvent(remainingResponse));
                break;
            case 0xd6:
                event.reset(new SSPHaltedEvent(remainingResponse));
                break;
            case 0xd7:
                event.reset(new SSPFloatingEvent(remainingResponse));
                break;
            case 0xd8:
                event.reset(new SSPFloatedEvent(remainingResponse));
                break;
            case 0xd9:
                event.reset(new SSPTimeoutEvent(remainingResponse));
                break;
            case 0xdc:
                event.reset(new SSPIncompletePayoutEvent(remainingResponse));
                break;
            case 0xdd:
                event.reset(new SSPIncompleteFloatEvent(remainingResponse));
                break;
            case 0xde:
                event.reset(new SSPCashboxPaidEvent(remainingResponse));
                break;
            case 0xdf:
                event.reset(new SSPCoinCreditEvent(remainingResponse));
                break;
            case 0xc4:
                event.reset(new SSPCoinMechJammedEvent(remainingResponse));
                break;
            case 0xc5:
                event.reset(new SSPCoinMechReturnPressedEvent(remainingResponse));
                break;
            case 0xc2:
                event.reset(new SSPEmptyingEvent(remainingResponse));
                break;
            case 0xc3:
                event.reset(new SSPEmptiedEvent(remainingResponse));
                break;
            case 0xb3:
                event.reset(new SSPSmartEmptyingEvent(remainingResponse));
                break;
            case 0xb4:
                event.reset(new SSPSmartEmptiedEvent(remainingResponse));
                break;
            case 0xb7:
                event.reset(new SSPCoinMechErrorEvent(remainingResponse));
                break;
            case 0xdb:
                event.reset(new SSPNoteStoredInPayoutEvent(remainingResponse));
                break;
            case 0xc6:
                event.reset(new SSPPayoutOutOfServiceEvent(remainingResponse));
                break;
            case 0xb0:
                event.reset(new SSPJamRecoveryEvent(remainingResponse));
                break;
            case 0xb1:
                event.reset(new SSPErrorDuringPayoutEvent(remainingResponse));
                break;
            case 0xc9:
                event.reset(new SSPNoteTransferredToStackerEvent(remainingResponse));
                break;
            case 0xce:
                event.reset(new SSPNoteHeldInBezelEvent(remainingResponse));
                break;
            case 0xcb:
                event.reset(new SSPNotePaidIntoStoreOnPowerupEvent(remainingResponse));
                break;
            case 0xca:
                event.reset(new SSPNotePaidIntoStackerOnPowerupEvent(remainingResponse));
                break;
            default:
                //throw "bla";
                qCritical() << "poll: unknown eventId:" << hex << eventId;
            }

            events.push_back(event);

            u_int8_t sizeDataLeft = remainingResponse.length() - event->length();
            remainingResponse = remainingResponse.right(sizeDataLeft);
        }
		
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
