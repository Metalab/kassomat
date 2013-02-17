#ifndef SMARTPAYOUT_H
#define SMARTPAYOUT_H

#include <QObject>

class SmartPayout : public QObject
{
    Q_OBJECT

    QString m_device;
    Q_PROPERTY(QString device READ device WRITE setDevice NOTIFY deviceChanged)

    int port;
public:
    explicit SmartPayout(QObject *parent = 0);
    ~SmartPayout();

    const QString &device() const;
    void setDevice(const QString &dev);

signals:
    void deviceChanged(const QString &newValue);
public slots:
};

#endif // SMARTPAYOUT_H
