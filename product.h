#ifndef PRODUCT_H
#define PRODUCT_H

#include "db/QDjangoModel.h"
class Product : public QDjangoModel
{

    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString project_id READ project_id WRITE setProject_id)
    Q_PROPERTY(QString display_name READ display_name WRITE setDisplay_name)
    Q_PROPERTY(QString barcode READ barcode WRITE setBarcode)
    Q_PROPERTY(qint32 cost READ cost WRITE setCost)

    Q_CLASSINFO("__meta__", "db_table=products")

    Q_CLASSINFO("id", "primary_key=true")
    Q_CLASSINFO("project_id", "on_delete=Projects.id")
    Q_CLASSINFO("display_name", "max_length=200")
    Q_CLASSINFO("foreign_id", "max_length=200")
    Q_CLASSINFO("barcode", "max_length=20")
    //Q_CLASSINFO("cost", " ")

    public:
        QString id() const{ return m_id; }
        QString project_id() const{ return m_project_id; }
        QString display_name() const{ return m_display_name; }
        QString barcode() const{ return m_barcode; }
        qint32 cost() const{ return m_cost; }

        void setId(const QString &id){ m_id = id; }
        void setProject_id(const QString &project_id){ m_project_id = project_id; }
        void setDisplay_name(const QString &display_name){ m_display_name = display_name; }
        void setBarcode(const QString &barcode){ m_barcode = barcode; }
        void setCost(const qint32 &cost){ m_cost = cost; }

    private:
        QString m_id;
        QString m_project_id;
        QString m_display_name;
        QString m_barcode;
        qint32 m_cost;
};
Q_DECLARE_METATYPE(Product*)

#endif // PRODUCT_H
