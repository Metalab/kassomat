#include "databasecontroller.h"
#include "db/QDjango.h"
#include "db/QDjangoQuerySet.h"
#include "user.h"
#include "mos_user.h"
#include "admin.h"
#include "booking.h"
#include "booking_row.h"
#include "product.h"
#include "project.h"
#include "genericmodel.h"


DatabaseController::DatabaseController(){

    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("kassomat");
    m_db.setUserName("vierlex");
    m_db.setPassword("kassomat");

    bool ok = m_db.open();


    if(ok) {
        QDjango::setDebugEnabled(true);
        QDjango::setDatabase(m_db);
        qDebug() << "yey!" ;

    } else {
        qDebug() << m_db.lastError();
        qDebug() << "fuckit";
    }

    QDjango::registerModel<User>();
    QDjango::registerModel<MOS_User>();
    QDjango::registerModel<Admin>();
    QDjango::registerModel<Moneycode>();
    QDjango::registerModel<Booking>();
    QDjango::registerModel<Booking_Row>();
    QDjango::registerModel<Product>();
    QDjango::registerModel<Project>();
}

QList<Product> DatabaseController::listProducts(){
    QDjangoQuerySet<Product> products;

//    QDjangoQuerySet<Project> someProjects;
//    someProjects = projects.filter(QDjangoWhere("name", QDjangoWhere::Equals, "*") &&
//                                    QDjangoWhere("valid_until", QDjangoWhere::NotEquals, "bla"));

    QList<Product> result;
    Product p;
    for (int i = 0; i < products.size(); ++i) {
        if (products.at(i, &p)) {
            result.append(p);
        }
    }

    return result;
}
