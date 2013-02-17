#ifndef KASSOMATCONTROLLER_H
#define KASSOMATCONTROLLER_H

#include <QObject>
#include "smartpayout.h"

class KassomatController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString smartPayoutDevice READ smartPayoutDevice WRITE setSmartPayoutDevice NOTIFY smartPayoutDeviceChanged)

    QScopedPointer<SmartPayout> m_smartPayout;
public:
    explicit KassomatController(QObject *parent = 0);

    const QString &smartPayoutDevice() const;
    void setSmartPayoutDevice(const QString &dev);

signals:

    void smartPayoutDeviceChanged(const QString &newDevice);
public slots:
    
};

#endif // KASSOMATCONTROLLER_H
