#include "kassomatcontroller.h"
#include <QDebug>

KassomatController::KassomatController(QObject *parent) :
    QObject(parent), m_smartPayout(new SmartPayout(this))
{
    QObject::connect(m_smartPayout.data(), &SmartPayout::deviceChanged, this, &KassomatController::smartPayoutDeviceChanged);
}

const QString &KassomatController::smartPayoutDevice() const {
    return m_smartPayout->device();
}

void KassomatController::setSmartPayoutDevice(const QString &dev) {
    m_smartPayout->setDevice(dev);
}
