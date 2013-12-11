#ifndef PROJECT_H
#define PROJECT_H

#include <QDateTime>
#include "QDjango/db/QDjangoModel.h"
#include "model/mos_user.h"

class Project : public QDjangoModel
{

    Q_OBJECT

    //Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString image_url READ image_url WRITE setImage_url)
    Q_PROPERTY(QString description READ description WRITE setDescription )
    Q_PROPERTY(QDateTime valid_until READ valid_until WRITE setValid_until )
    Q_PROPERTY(qint32 funding_target_min READ funding_target_min WRITE setFunding_target_min )
    Q_PROPERTY(qint32 funding_target_max READ funding_target_max WRITE setFunding_target_max )
    Q_PROPERTY(MOS_User* mos_user READ mos_user WRITE setMOS_User)
    Q_PROPERTY(QString admin_password READ admin_password WRITE setAdmin_password)
    Q_PROPERTY(QString notification_email READ notification WRITE setNotification)
    Q_PROPERTY(qint32 sort_order READ sort_order WRITE setSort_order)

    Q_CLASSINFO("__meta__", "db_table=projects")

    //("id", "primary_key=true")
    Q_CLASSINFO("name", "max_length=100")
    Q_CLASSINFO("image_url", "max_length=200")
    Q_CLASSINFO("description", "max_length=2000")
    //Q_CLASSINFO("valid_until", "")
    //Q_CLASSINFO("funding_target_min", "")
    //Q_CLASSINFO("funding_target_max", "")
    Q_CLASSINFO("admin_password", "max_length=100")
    Q_CLASSINFO("notification_email", "max_length=100")
    //Q_CLASSINFO("sort_order", " ")

    public:
        Project(QObject *parent=0)
            : QDjangoModel(parent)
        {
            setForeignKey("mos_user", new MOS_User(this));
        }

        // id() const{ return m_id; }
        QString name() const{ return m_name; }
        QString image_url() const{ return m_image_url; }
        QString description() const{ return m_description; }
        QDateTime valid_until() const{ return m_valid_until; }
        qint32 funding_target_min() const{ return m_funding_target_min; }
        qint32 funding_target_max() const{ return m_funding_target_max; }
        MOS_User* mos_user() const{ return qobject_cast<MOS_User*>(foreignKey("mos_user")); }
        QString admin_password() const{ return m_admin_password; }
        QString notification() const{ return m_notification; }
        qint32 sort_order() const{ return m_sort_order; }

        //void setId(const qint64 &id){ m_id = id; }
        void setName(const QString &name){ m_name = name; }
        void setImage_url(const QString &image_url){ m_image_url = image_url; }
        void setDescription(const QString &description){ m_description = description; }
        void setValid_until(const QDateTime &valid_until){ m_valid_until = valid_until; }
        void setFunding_target_min(const qint32 &funding_target_min){ m_funding_target_min = funding_target_min; }
        void setFunding_target_max(const qint32 &funding_target_max){ m_funding_target_max = funding_target_max; }
        void setMOS_User(MOS_User *mos_user){ setForeignKey("mos_user", mos_user); }
        void setAdmin_password(const QString &admin_password){ m_admin_password = admin_password; }
        void setNotification(const QString &notification){ m_notification =  notification; }
        void setSort_order(const qint32 &sort_order){ m_sort_order = sort_order; }

    private:
        //qint64 m_id;
        QString m_name;
        QString m_image_url;
        QString m_description;
        QDateTime m_valid_until;
        qint32 m_funding_target_min;
        qint32 m_funding_target_max;
        MOS_User* m_mos_user;
        QString m_admin_password;
        QString m_notification;
        qint32 m_sort_order;
};

Q_DECLARE_METATYPE(MOS_User*)


#endif // PROJECT_H
