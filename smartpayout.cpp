#include "smartpayout.h"
#include <QDebug>
#include "sspcoms.h"
#include "sspevents.h"

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
            qDebug() << "Opening device" << dev;
			coms = new SSPComs(QSerialPortInfo(dev));
			QObject::connect(coms, &SSPComs::terminating, [=]{
				delete coms;
			});
			
			coms->startConnection();
			
            coms->enable([]() {
                qDebug() << "enable finished";
            });

			coms->datasetVersion([](const QString &datasetVersion){
				qDebug() << "datasetVersion =" << datasetVersion;
			});

            coms->firmwareVersion([](const QString &firmwareVersion){
                qDebug() << "firmwareVersion =" << firmwareVersion;
            });
        } else {
			coms = nullptr;
        }
        emit deviceChanged(m_device);
    }
}

void SmartPayout::test() {
    coms->enable([]() {
        qDebug() << "enable finished";
    });

//    coms->emptyAll([]() {
//        qDebug() << "empty all finished";
//    });

    coms->displayOn([]() {
        qDebug() << "displayOn finished";
    });
}

void SmartPayout::poll() {
    coms->poll([](const QList<QSharedPointer<SSPEvent>> &events){
        qDebug() << "poll callback: got" << events.length() << "event(s)";
        for(int i = 0; i < events.length(); i++) {
            SSPEvent *event = events[i].data();
            qDebug() << "poll callback: received: " << event->eventDescription();
        }
    });
}
