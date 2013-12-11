#ifndef MONEYCODE_H
#define MONEYCODE_H

#include <QDateTime>
#include "QDjango/db/QDjangoModel.h"
#include "model/booking.h"
#include "model/user.h"

class Moneycode : public QDjangoModel
{

    Q_OBJECT

//    Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(User* user READ user WRITE setUser)
    Q_PROPERTY(Booking* booking_in READ booking_in WRITE setBooking_in)
    Q_PROPERTY(Booking* booking_out READ booking_out WRITE setBooking_out)
    Q_PROPERTY(QString code READ code WRITE setCode)
    Q_PROPERTY(QDateTime valid_until READ valid_until WRITE setValid_until )

    Q_CLASSINFO("__meta__", "db_table=moneycodes")

//    Q_CLASSINFO("id", "primary_key=true")
    //Q_CLASSINFO("user", "on_delete=Users.id")
    //Q_CLASSINFO("booking_in", "on_delete=Bookings.id")
    //Q_CLASSINFO("booking_out", "on_delete=Bookings.id")
    Q_CLASSINFO("code", "max_length=100")

    public:
    Moneycode(QObject *parent=0)
        : QDjangoModel(parent)
    {
        setForeignKey("booking_in", new Booking(this));
        setForeignKey("booking_out", new Booking(this));
        setForeignKey("user", new User(this));
    }

//    qint64 id() const{ return m_id; }
    User* user() const{ return qobject_cast<User*>(foreignKey("user")); }
    Booking* booking_in() const{ return qobject_cast<Booking*>(foreignKey("booking_in")); }
    Booking* booking_out() const{ return qobject_cast<Booking*>(foreignKey("booking_out")); }
    QString code() const{ return m_code; }
    QDateTime valid_until() const{ return m_valid_until; }

//    void setId(const qint64 &id){ m_id = id; }
    void setUser(User *user){ setForeignKey("user", user); }
    void setBooking_in(Booking *booking){ setForeignKey("booking_in", booking); }
    void setBooking_out(Booking *booking){ setForeignKey("booking_out", booking); }
    void setCode(const QString &code){ m_code = code; }
    void setValid_until(const QDateTime &valid_until){ m_valid_until = valid_until; }

    private:
//        qint64 m_id;
        QString m_code;
        QDateTime m_valid_until;
};

//declared at booking row, should i move all declares to one place?
//Q_DECLARE_METATYPE(Booking*)
Q_DECLARE_METATYPE(User*)

#endif // MONEYCODE_H
