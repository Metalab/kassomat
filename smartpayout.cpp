#include "smartpayout.h"
#include <QDebug>
extern "C" {
#include "itlssp/serialfunc.h"
}

SmartPayout::SmartPayout(QObject *parent) :
    QObject(parent), port(0) {
}

SmartPayout::~SmartPayout() {
    if(port != 0) {
        CloseSSPPort(port);
    }
}

const QString &SmartPayout::device() const {
    return m_device;
}

void SmartPayout::setDevice(const QString &dev) {
    if(m_device != dev) {
        m_device = dev;

        if(port != 0) {
            CloseSSPPort(port);
        }

        if(dev != "") {
            port = OpenSSPPort(dev.toUtf8());

            if(port == -1) {
                qFatal("Failed opening device");
                return;
            }
        } else {
            port = 0;
        }
        emit deviceChanged(m_device);
    }
}
