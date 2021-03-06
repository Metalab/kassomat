#ifndef USER_H
#define USER_H

#include "QDjango/db/QDjangoModel.h"

class User : public QDjangoModel{

    Q_OBJECT

    //Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(QString email READ email WRITE setEmail)

    Q_CLASSINFO("__meta__", "db_table=users")

    //Q_CLASSINFO("id", "primary_key=true")
    Q_CLASSINFO("email", "max_length=200")

    public:
    User(QObject *parent=0)
        : QDjangoModel(parent)
    {

    }

    //qint64 id() const{ return m_id; }
    QString email() const{ return m_email; }

    //void setId(const qint64 &id){ m_id = id; }
    void setEmail(const QString &email){ m_email = email; }

    private:
        //qint64 m_id;
        QString m_email;
};

#endif // USER_H
