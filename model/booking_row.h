#ifndef BOOKING_ROW_H
#define BOOKING_ROW_H

#include "QDjango/db/QDjangoModel.h"
#include "model/booking.h"
#include "model/product.h"


class Booking_Row : public QDjangoModel
{
    Q_OBJECT

//    Q_PROPERTY(qint64 id READ id WRITE setId)
    Q_PROPERTY(Booking* booking READ booking WRITE setBooking)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(QString product_foreign_id READ product_foreign_id WRITE setProduct_foreign_id)
    Q_PROPERTY(qint32 cost_per_item READ cost_per_item WRITE setCost_per_item)
    Q_PROPERTY(qint32 amount READ amount WRITE setAmount)
    Q_PROPERTY(Product* product READ product WRITE setProduct)

    Q_CLASSINFO("__meta__", "db_table=booking_rows")

//    Q_CLASSINFO("id", "primary_key=true")
    //Q_CLASSINFO("booking", "on_delete=Bookings.id")
    Q_CLASSINFO("text", "max_length=200")
    Q_CLASSINFO("product_foreign_id", "max_length=200")
    //Q_CLASSINFO("cost_per_item", " ")
    //Q_CLASSINFO("amount", " ")
    //Q_CLASSINFO("project", " ")

    public:
        Booking_Row(QObject *parent=0)
            : QDjangoModel(parent)
        {
            setForeignKey("booking", new Booking(this));
            setForeignKey("product", new Product(this));
        }

//        qint64 id() const{ return m_id; }
        Booking* booking() const{ return qobject_cast<Booking*>(foreignKey("booking")); }
        QString text() const{ return m_text; }
        QString product_foreign_id() const{ return m_product_foreign_id; }
        qint32 cost_per_item() const{ return m_cost_per_item; }
        qint32 amount() const{ return m_amount; }
        Product* product() const{ return qobject_cast<Product*>(foreignKey("product")); }


//        void setId(const qint64 &id){ m_id = id; }
        void setBooking(Booking* booking){ setForeignKey("booking", booking); }
        void setText(const QString &text){ m_text = text; }
        void setProduct_foreign_id(const QString &product_foreign_id){ m_product_foreign_id = product_foreign_id; }
        void setCost_per_item(const qint32 &cost_per_item){ m_cost_per_item = cost_per_item; }
        void setAmount(const qint32 &amount){ m_amount = amount; }
        void setProduct(Product* product){ setForeignKey("product", product); }

    private:
//        qint64 m_id;
        QString m_text;
        QString m_product_foreign_id;
        qint32 m_cost_per_item;
        qint32 m_amount;
};

Q_DECLARE_METATYPE(Booking*)
Q_DECLARE_METATYPE(Product*)

#endif // BOOKING_ROW_H
