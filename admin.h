#ifndef ADMINS_H
#define ADMINS_H


#include "db/QDjangoModel.h"
class Admins : public QDjangoModel
{

    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString password READ password WRITE setPassword)

    Q_CLASSINFO("id", "primary_key=true")
    Q_CLASSINFO("name", "max_length=100")
    Q_CLASSINFO("password", "max_length=100")

    public:
    QString id() const{ return m_id; }
    QString name() const{ return m_name; }
    QString password() const{ return m_password; }

    void setId(const QString &id){ m_id = id; }
    void setName(const QString &name){ m_name = name; }
    void setPassword(const QString &password){ m_password = password; }

    private:
        QString m_id;
        QString m_name;
        QString m_password;
};



#endif // ADMINS_H
