#ifndef ADMIN_H
#define ADMIN_H

#include "mos_user.h"

class Admin : public QDjangoModel{

    Q_OBJECT

    Q_PROPERTY(MOS_User* mos_user READ mos_user WRITE setMOS_User)
    Q_PROPERTY(QString password READ password WRITE setPassword)

    Q_CLASSINFO("__meta__", "db_table=admins")

    Q_CLASSINFO("password", "max_length=100")

    public:
    Admin(QObject *parent=0)
        : QDjangoModel(parent)
    {
        setForeignKey("mos_user", new MOS_User(this));
    }

    MOS_User* mos_user() const{ return qobject_cast<MOS_User*>(foreignKey("mos_user")); }
    QString password() const{ return m_password; }

    void setMOS_User(MOS_User* mos_user){ setForeignKey("mos_user", mos_user); }
    void setPassword(const QString &password){ m_password = password; }

    private:
        QString m_password;

};

//Q_DECLARE_METATYPE(MOS_User*)

#endif // ADMIN_H
