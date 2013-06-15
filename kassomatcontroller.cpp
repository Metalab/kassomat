#include "kassomatcontroller.h"
#include <QDebug>

KassomatController::KassomatController(QObject *parent) :
    QObject(parent), m_smartPayout(new SmartPayout(this))
{
    QObject::connect(m_smartPayout.data(), &SmartPayout::deviceChanged, this, &KassomatController::smartPayoutDeviceChanged);
    m_credit=1100;
    m_state="STANDARD_SCREEN";
}

const QString &KassomatController::smartPayoutDevice() const {
    return m_smartPayout->device();
}

void KassomatController::setSmartPayoutDevice(const QString &dev) {
    m_smartPayout->setDevice(dev);
    this->
}

unsigned int KassomatController::credit() const{
    return m_credit;
}

const QString& KassomatController::state() const{
    return m_state;
}

void KassomatController::setCredit(unsigned int credit){
    m_credit = credit;
    emit creditChanged(credit);
}

void KassomatController::setState(const QString &state){
    m_state = state;
    emit stateChanged(state);
}
