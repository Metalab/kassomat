#ifndef PRODUCT_H
#define PRODUCT_H

#include "QDjango/db/QDjangoModel.h"
#include "project.h"

class Product : public QDjangoModel
{

    Q_OBJECT

    //Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(Project* project READ project WRITE setProject)
    Q_PROPERTY(QString display_name READ display_name WRITE setDisplay_name)
    Q_PROPERTY(QString image_url READ image_url WRITE setImage_url)
    Q_PROPERTY(QString foreign_id READ foreign_id WRITE setForeign_id)
    Q_PROPERTY(QString barcode READ barcode WRITE setBarcode)
    Q_PROPERTY(qint32 cost READ cost WRITE setCost)

    Q_CLASSINFO("__meta__", "db_table=products")

    //Q_CLASSINFO("id", "primary_key=true")
    //Q_CLASSINFO("project", "on_delete=Projects.id")
    Q_CLASSINFO("display_name", "max_length=200")
    Q_CLASSINFO("image_url", "max_length=200")
    Q_CLASSINFO("foreign_id", "max_length=200")
    Q_CLASSINFO("barcode", "max_length=20")
    //Q_CLASSINFO("cost", " ")

    public:
        Product(QObject *parent=0)
            : QDjangoModel(parent)
        {
            setForeignKey("project", new Project(this));
        }

//        qint64 id() const{ return m_id; }
        Project* project() const{ return qobject_cast<Project*>(foreignKey("project")); }
        QString display_name() const{ return m_display_name; }
        QString image_url() const{ return m_image_url; }
        QString foreign_id() const{ return m_foreign_id; }
        QString barcode() const{ return m_barcode; }
        qint32 cost() const{ return m_cost; }

//        void setId(const qint64 &id){ m_id = id; }
        void setProject(Project* project){ setForeignKey("project", project); }
        void setDisplay_name(const QString &display_name){ m_display_name = display_name; }
        void setImage_url(const QString &image_url){ m_image_url = image_url; }
        void setForeign_id(const QString &foreign_id){ m_foreign_id = foreign_id; }
        void setBarcode(const QString &barcode){ m_barcode = barcode; }
        void setCost(const qint32 &cost){ m_cost = cost; }

    private:
//        qint64 m_id;
        QString m_display_name;
        QString m_image_url;
        QString m_foreign_id;
        QString m_barcode;
        qint32 m_cost;
};

Q_DECLARE_METATYPE(Project*)

#endif // PRODUCT_H
