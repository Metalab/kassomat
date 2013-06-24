#ifndef MONEYCODE_H
#define MONEYCODE_H

#include <QDateTime>
#include "db/QDjangoModel.h"
class Moneycode : public QDjangoModel
{

    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString user_id READ user_id WRITE setUser_id)
    Q_PROPERTY(QString booking_id_in READ booking_id_in WRITE setBooking_id_in)
    Q_PROPERTY(QString booking_id_out READ booking_id_out WRITE setBooking_id_out)
    Q_PROPERTY(QString code READ code WRITE setCode)
    Q_PROPERTY(QDateTime valid_until READ valid_until WRITE setValid_until )

    Q_CLASSINFO("__meta__", "db_table=moneycodes")

    Q_CLASSINFO("id", "primary_key=true")
    Q_CLASSINFO("user_id", "on_delete=Users.id")
    Q_CLASSINFO("booking_id_in", "on_delete=Bookings.id")
    Q_CLASSINFO("booking_id_out", "on_delete=Bookings.id")
    Q_CLASSINFO("code", "max_length=100")

    public:
    QString id() const{ return m_id; }
    QString user_id() const{ return m_user_id; }
    QString booking_id_in() const{ return m_booking_id_in; }
    QString booking_id_out() const{ return m_booking_id_out; }
    QString code() const{ return m_code; }
    QDateTime valid_until() const{ return m_valid_until; }

    void setId(const QString &id){ m_id = id; }
    void setUser_id(const QString &user_id){ m_user_id = user_id; }
    void setBooking_id_in(const QString &booking_id_in){ m_booking_id_in = booking_id_in; }
    void setBooking_id_out(const QString &booking_id_out){ m_booking_id_out = booking_id_out; }
    void setCode(const QString &code){ m_code = code; }
    void setValid_until(const QDateTime &valid_until){ m_valid_until = valid_until; }

    private:
        QString m_id;
        QString m_user_id;
        QString m_booking_id_in;
        QString m_booking_id_out;
        QString m_code;
        QDateTime m_valid_until;
};

#endif // MONEYCODE_H
