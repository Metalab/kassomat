#include "kassomatcontroller.h"

KassomatController::KassomatController(QObject *parent) :
    QObject(parent)
{


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
