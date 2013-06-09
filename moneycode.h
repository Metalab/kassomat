#ifndef MONEYCODES_H
#define MONEYCODES_H

#include "db/QDjangoModel.h"
class Moneycode : public QDjangoModel
{

    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString user_id READ user_id WRITE setUser_id)
    Q_PROPERTY(QString code READ code WRITE setCode)

    Q_CLASSINFO("__meta__", "db_table=moneycodes")

    Q_CLASSINFO("id", "primary_key=true")
    Q_CLASSINFO("user_id", "on_delete=Users.id")
    Q_CLASSINFO("code", "max_length=100")

    public:
    QString id() const{ return m_id; }
    QString user_id() const{ return m_user_id; }
    QString code() const{ return m_code; }

    void setId(const QString &id){ m_id = id; }
    void setUser_id(const QString &user_id){ m_user_id = user_id; }
    void setCode(const QString &code){ m_code = code; }

    private:
        QString m_id;
        QString m_user_id;
        QString m_code;
};

#endif // MONEYCODES_H
