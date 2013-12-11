#ifndef MOS_USER_H
#define MOS_USER_H

#include "user.h"

class MOS_User : public QDjangoModel{

    Q_OBJECT

    Q_PROPERTY(User* user READ user WRITE setUser)
    Q_PROPERTY(QString mos_id READ mos_id WRITE setMos_id)
    Q_PROPERTY(QString ibutton_id READ ibutton_id WRITE setIbutton_id)

    Q_CLASSINFO("__meta__", "db_table=mos_users")

    Q_CLASSINFO("user", "unique=true")
    Q_CLASSINFO("users.id", "unique=true")
    Q_CLASSINFO("mos_users.user_id", "unique=true")
    Q_CLASSINFO("users_id", "unique=true")
    Q_CLASSINFO("User.id", "unique=true")
    Q_CLASSINFO("Users.id", "unique=true")

    Q_CLASSINFO("mos_id", "max_length=100")
    Q_CLASSINFO("ibutton_id", "max_length=20")

    public:
    MOS_User(QObject *parent=0)
        : QDjangoModel(parent)
    {
        setForeignKey("user", new User(this));

    }

    User* user()const{ return qobject_cast<User*>(foreignKey("user")); }
    QString mos_id() const{ return m_mos_id; }
    QString ibutton_id() const{ return m_ibutton_id; }

    void setUser(User* user){ setForeignKey("user", user); }
    void setMos_id(const QString &mos_id){ m_mos_id = mos_id; }
    void setIbutton_id(const QString &ibutton_id){ m_ibutton_id = ibutton_id; }

    private:
    QString m_mos_id;
    QString m_ibutton_id;

};

//Q_DECLARE_METATYPE(User*)

#endif // MOS_USER_H
