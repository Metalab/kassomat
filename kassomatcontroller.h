#ifndef KASSOMATCONTROLLER_H
#define KASSOMATCONTROLLER_H

#include <QObject>

class KassomatController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int credit READ credit WRITE setCredit NOTIFY creditChanged)
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)

public:
    explicit KassomatController(QObject *parent = 0);

    unsigned int credit() const;
    const QString& state() const;

    void setCredit(unsigned int credit);
    const void setState(const QString &state);

signals:
    void creditChanged(unsigned int);
    void stateChanged(QString);

public slots:

private:
    unsigned int m_credit;
    QString m_state;
};

#endif // KASSOMATCONTROLLER_H
