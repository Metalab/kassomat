#ifndef SSPCOMS_H
#define SSPCOMS_H

#include <functional>

#include <QList>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QtSerialPort/QtSerialPort>
#include <openssl/aes.h>
#include <openssl/bn.h>

class SSPComsTask;
class SSPEvent;

class TimeoutException {};

class SSPComs : public QThread {
    Q_OBJECT
	
	Q_PROPERTY(bool terminate READ terminate WRITE setTerminate)
	
    QSerialPort *m_port;
	QSerialPortInfo m_portInfo;

public:
    explicit SSPComs(const QSerialPortInfo &info);
    ~SSPComs();

    enum Result {
        ok = 0xf0,
        commandNotKnown = 0xf2,
        wrongNumberOfParameters = 0xf3,
        parameterOutOfRange = 0xf4,
        commandCannotBeProcessed = 0xf5,
        softwareError = 0xf6,
        fail = 0xf8,
        keyNotSet = 0xfa
    };
    enum Result_Payout {
        payoutOk = -1,
        notEnoughValue = 0,
        cannotPayAmount = 1,
        deviceBusy = 3,
        deviceDisabled = 4
    };
    enum Result_Route {
        deviceError = -3,
        invalidCurrency = -2,
        notConnected = -1,
        recycled = 0,
        cashbox = 1
    };
	
	void startConnection();
	
	bool terminate() const { return m_terminate; }
	void setTerminate(bool terminate);
	
    void reset(std::function<void()> callback);
    void disable(std::function<void()> callback);
    void enable(std::function<void()> callback);
    void datasetVersion(std::function<void(const QString&)> callback);
    void firmwareVersion(std::function<void(const QString&)> callback);
    void emptyAll(std::function<void()> callback);
    void displayOn(std::function<void()> callback);
    void displayOff(std::function<void()> callback);

    Result_Payout payout(uint32_t amount, bool test=false);
    uint32_t getDenominationLevel(uint32_t denomination, const QString &currency);
    Result_Route getDenominationRoute(uint32_t denomination, const QString &currency);
    uint32_t getMinimalPayout(const QString &currency);
    void setCoinInhibits(bool inhibited, uint32_t denomination, const QString &currency);

    struct PayoutDenomination {
        uint32_t number;
        uint32_t denomination; // in cents
        QString currency;
    };

    Result_Payout payoutByDenomination(const QList<PayoutDenomination> &requests);
    void smartEmpty();
    void poll(std::function<void(QList<QSharedPointer<SSPEvent>>)> callback);
    uint32_t getSerialNumber();
    QString getFirmwareVersion();
    const QList<PayoutDenomination> getAllLevels();
    bool setDenominationLevel(uint32_t numToAdd, uint32_t denomination, const QString &currency);
    void haltPayout();
    Result_Payout floatAmount(unsigned minPayout, uint32_t totalAmount, const QString &currency, bool test=false);
    void emptyAll();
    void setCoinMechGlobalInhibit(bool enable);
    QList<PayoutDenomination> cashboxPayoutOperationData(); // after smartEmpty, emptyAll, floatAmount, "dispense"

protected:
    void run();
signals:
	void terminating();
private:
	bool m_terminate;
	
	QQueue<SSPComsTask*> m_taskQueue;
	QMutex m_taskQueueMutex;
	QWaitCondition m_taskQueueUpdatedCondition;
    bool m_sequence;

	bool open();

	void enqueueTask(const QByteArray &data, const std::function<void(uint8_t, const QByteArray&)> &response);
	QByteArray readResponse();
    bool sendCommand(uint8_t slave_id, const QByteArray &cmd, bool retry=false);
    uint16_t calculateCRC(const QByteArray &p, uint16_t seed, uint16_t cd);

    bool sync();

	// encryption
	
    bool m_encryptionEnabled;
    QByteArray m_key;
    uint32_t m_encryptionCount;
    QByteArray encrypt(const QByteArray &cmd, bool retry);
    QByteArray decrypt(const QByteArray &cmd);
    void negotiateEncryption(uint64_t fixedKey);
    BN_CTX *m_bn_ctx;
	
	Q_DISABLE_COPY(SSPComs)
};

#endif // SSPCOMS_H
