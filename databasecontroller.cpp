#include "databasecontroller.h"
#include "db/QDjango.h"
#include "user.h"
#include "mos_user.h"
#include "admin.h"
#include "booking.h"
#include "booking_row.h"
#include "product.h"
#include "project.h"


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
        qDebug() << "yey?" ;

    } else {
        qDebug() << "fuckit";
    }

    QDjango::registerModel<User>();
    QDjango::registerModel<MOS_User>();
    QDjango::registerModel<Admin>();
    QDjango::registerModel<Booking>();
    QDjango::registerModel<Booking_Row>();
    QDjango::registerModel<Product>();
    QDjango::registerModel<Project>();
}
