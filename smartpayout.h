#ifndef SMARTPAYOUT_H
#define SMARTPAYOUT_H

#include <QObject>

class SSPComs;

class SmartPayout : public QObject
{
    Q_OBJECT

    QString m_device;
    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged)

	SSPComs *coms;
public:
    explicit SmartPayout(QObject *parent = 0);
    ~SmartPayout();

    const QString &device() const;
    void setDevice(const QString &dev);

    void test();

signals:
    void deviceChanged(const QString &newValue);
};

#endif // SMARTPAYOUT_H
