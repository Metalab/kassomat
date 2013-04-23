#ifndef BOOKING_H
#define BOOKING_H

#include <QDateTime>
#include "db/QDjangoModel.h"
class Booking : public QDjangoModel
{

    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QDateTime booking_time READ booking_time WRITE setBooking_time)
    Q_PROPERTY(qint64 admin_id READ admin_id WRITE setAdmin_id)
    Q_PROPERTY(QString user_email READ user_email WRITE setUser_email)

    Q_CLASSINFO("__meta__", "db_table=bookings")

    Q_CLASSINFO("id", "primary_key=true")
    //Q_CLASSINFO("booking_time", "")
    //Q_CLASSINFO("admin_id", "")
    Q_CLASSINFO("user_email", "max_length=200")

    public:
        QString id() const{ return m_id; }
        QDateTime booking_time() const{ return m_booking_time; }
        qint64 admin_id() const{ return m_admin_id; }
        QString user_email() const{ return m_user_email; }

        void setId(const QString &id){ m_id = id; }
        void setBooking_time(const QDateTime &booking_time){ m_booking_time = booking_time; }
        void setAdmin_id(const qint64 &admin_id){ m_admin_id = admin_id; }
        void setUser_email(const QString &user_email){ m_user_email = user_email; }

    private:
        QString m_id;
        QDateTime m_booking_time;
        qint64 m_admin_id;
        QString m_user_email;
};

#endif // BOOKING_H
