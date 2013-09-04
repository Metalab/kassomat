#ifndef BOOKING_H
#define BOOKING_H

#include <QDateTime>
#include "db/QDjangoModel.h"
#include "admin.h"

class Booking : public QDjangoModel
{

    Q_OBJECT

//    Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(QDateTime booking_time READ booking_time WRITE setBooking_time)
    Q_PROPERTY(Admin* admin READ admin WRITE setAdmin)
    Q_PROPERTY(QString user_email READ user_email WRITE setUser_email)

    Q_CLASSINFO("__meta__", "db_table=bookings")

//    Q_CLASSINFO("id", "primary_key=true")
    //Q_CLASSINFO("booking_time", "")
    //Q_CLASSINFO("admin_id", "")
    Q_CLASSINFO("user_email", "max_length=200")

    public:
        Booking(QObject *parent=0)
            : QDjangoModel(parent)
        {
            setForeignKey("admin", new Admin(this));
        }

//        qint64 id() const{ return m_id; }
        QDateTime booking_time() const{ return m_booking_time; }
        Admin* admin() const{ return qobject_cast<Admin*>(foreignKey("admin")); }
        QString user_email() const{ return m_user_email; }

//        void setId(const qint64 &id){ m_id = id; }
        void setBooking_time(const QDateTime &booking_time){ m_booking_time = booking_time; }
        void setAdmin(Admin* admin){ setForeignKey("admin", admin); }
        void setUser_email(const QString &user_email){ m_user_email = user_email; }

    private:
//        qint64 m_id;
        QDateTime m_booking_time;
        QString m_user_email;
};

Q_DECLARE_METATYPE(Admin*)

#endif // BOOKING_H
