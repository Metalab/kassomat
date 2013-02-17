#ifndef KASSOMATCONTROLLER_H
#define KASSOMATCONTROLLER_H

#include <QObject>
#include "smartpayout.h"

class KassomatController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString smartPayoutDevice READ smartPayoutDevice WRITE setSmartPayoutDevice NOTIFY smartPayoutDeviceChanged)

    QScopedPointer<SmartPayout> m_smartPayout;
    Q_PROPERTY(unsigned int credit READ credit WRITE setCredit NOTIFY creditChanged)
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)

public:
    explicit KassomatController(QObject *parent = 0);

    const QString &smartPayoutDevice() const;
    void setSmartPayoutDevice(const QString &dev);

    unsigned int credit() const;
    const QString& state() const;

    void setCredit(unsigned int credit);
    const void setState(const QString &state);

signals:

    void smartPayoutDeviceChanged(const QString &newDevice);
    void creditChanged(unsigned int);
    void stateChanged(QString);

public slots:

private:
    unsigned int m_credit;
    QString m_state;
};

#endif // KASSOMATCONTROLLER_H
