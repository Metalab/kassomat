#include "kassomatcontroller.h"
#include <QDebug>

KassomatController::KassomatController(QObject *parent) :
    QObject(parent), m_smartPayout(new SmartPayout(this))
{
    QObject::connect(m_smartPayout.data(), &SmartPayout::deviceChanged, this, &KassomatController::smartPayoutDeviceChanged);
    m_state="STANDARD_SCREEN";
    m_credit=0;
}

const QString &KassomatController::smartPayoutDevice() const {
    return m_smartPayout->device();
}

unsigned int KassomatController::credit() const{
    return m_credit;
}

unsigned int KassomatController::debt() const{
    return m_debt;
}

const QString& KassomatController::state() const{
    return m_state;
}

void KassomatController::setSmartPayoutDevice(const QString &dev) {
    m_smartPayout->setDevice(dev);
}

void KassomatController::setCredit(unsigned int credit){
    m_credit = credit;
    emit creditChanged(credit);
}

void KassomatController::setDebt(unsigned int debt){
    m_debt = debt;
    emit debtChanged(debt);
}

void KassomatController::setState(const QString &state){
    m_state = state;
    emit stateChanged(state);
}
