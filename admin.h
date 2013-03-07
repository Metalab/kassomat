#ifndef ADMIN_H
#define ADMIN_H

#include "mos_user.h"

class Admin : public MOS_User
{

    Q_OBJECT

    Q_PROPERTY(QString password READ password WRITE setPassword)

    Q_CLASSINFO("__meta__", "db_table=admins")

    Q_CLASSINFO("password", "max_length=100")

    public:
    QString password() const{ return m_password; }

    void setPassword(const QString &password){ m_password = password; }

    private:
        QString m_password;

};



#endif // ADMIN_H
