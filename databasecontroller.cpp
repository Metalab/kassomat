#include "databasecontroller.h"
#include "db/QDjango.h"
#include "admins.h"
#include "bookings.h"
#include "booking_rows.h"
#include "products.h"
#include "projects.h"


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

    QDjango::registerModel<Admins>();
    QDjango::registerModel<Bookings>();
    QDjango::registerModel<Booking_Rows>();
    QDjango::registerModel<Products>();
    QDjango::registerModel<Projects>();
}
