#ifndef SMARTPAYOUT_H
#define SMARTPAYOUT_H

#include <QObject>

class SmartPayout : public QObject
{
    Q_OBJECT
public:
    explicit SmartPayout(QObject *parent = 0);

signals:
public slots:
    void testing();
};

#endif // SMARTPAYOUT_H
