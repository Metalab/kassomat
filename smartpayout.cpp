#include "smartpayout.h"

SmartPayout::SmartPayout(QObject *parent) :
    QObject(parent)
{
}

void SmartPayout::testing() {
    qDebug("donate!");
}
