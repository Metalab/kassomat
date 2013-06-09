#include "smartpayout.h"
#include <QDebug>
#include "sspcoms.h"

SmartPayout::SmartPayout(QObject *parent) :
    QObject(parent), coms(nullptr) {
}

SmartPayout::~SmartPayout() {
    if(coms) {
		
    }
}

const QString &SmartPayout::device() const {
    return m_device;
}

void SmartPayout::setDevice(const QString &dev) {
    if(m_device != dev) {
        m_device = dev;

        if(coms != nullptr)
			coms->setTerminate(true);

        if(dev != "") {
			coms = new SSPComs(QSerialPortInfo(dev));
			QObject::connect(coms, &SSPComs::terminating, [=]{
				delete coms;
			});
			
			coms->startConnection();
			
			coms->datasetVersion([](const QString &datasetVersion){
				qDebug() << "datasetVersion =" << datasetVersion;
			});
        } else {
			coms = nullptr;
        }
        emit deviceChanged(m_device);
    }
}
