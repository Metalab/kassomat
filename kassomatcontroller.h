#ifndef KASSOMATCONTROLLER_H
#define KASSOMATCONTROLLER_H

#include <QObject>
#include "smartpayout.h"

class KassomatController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString smartPayoutDevice READ smartPayoutDevice WRITE setSmartPayoutDevice NOTIFY smartPayoutDeviceChanged)
    Q_PROPERTY(unsigned int credit READ credit WRITE setCredit NOTIFY creditChanged)
    Q_PROPERTY(unsigned int debt READ debt WRITE setDebt NOTIFY debtChanged)
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)

public:
    explicit KassomatController(QObject *parent = 0);

    const QString& smartPayoutDevice() const;
    unsigned int credit() const;
    const QString& state() const;
    unsigned int debt() const;

    void setSmartPayoutDevice(const QString &dev);
    void setCredit(unsigned int credit);
    void setState(const QString &state);
    void setDebt(unsigned int debt);

signals:
    void smartPayoutDeviceChanged(const QString &newDevice);
    void creditChanged(unsigned int);
    void debtChanged(unsigned int);
    void stateChanged(QString);

public slots:

private:
    QScopedPointer<SmartPayout> m_smartPayout;
    unsigned int m_credit;
    unsigned int m_debt;
    QString m_state;
};

#endif // KASSOMATCONTROLLER_H
