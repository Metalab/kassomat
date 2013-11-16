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
#include "log.h"


DatabaseController::DatabaseController(){

    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName("localhost");
    m_db.setDatabaseName("kassomat");
    m_db.setUserName("vierlex");
    m_db.setPassword("kassomat");

    if(m_db.open()) {
        QDjango::setDatabase(m_db);
        dbTestFill();
    } else {
        qDebug() << m_db.lastError();
        qDebug() << "fuckit, no connection?";
    }
}

void DatabaseController::dbTestFill(){
    qDebug() << "database connection established" ;

    QDjango::setDebugEnabled(true);
    QDjango::registerModel<User>().createTable();  
    QDjango::registerModel<MOS_User>().createTable();
    QDjango::registerModel<Admin>().createTable();
    QDjango::registerModel<Project>().createTable();
    QDjango::registerModel<Product>().createTable();
    QDjango::registerModel<Booking>().createTable();
    QDjango::registerModel<Booking_Row>().createTable();
    QDjango::registerModel<Moneycode>().createTable();
    QDjango::registerModel<Log>().createTable();

    User admin_user;
    //QDjangoQuerySet<User> u;
    //admin_user = u.at(0);
    admin_user.setEmail("alexander-sulz@gmx.at");
    admin_user.save();

    MOS_User admin_mos;
    admin_mos.setIbutton_id("33-0000056478");
    admin_mos.setMos_id("234");
    admin_mos.setUser(&admin_user);
    admin_mos.save();

    Admin admin_admin;
    admin_admin.setPassword("kassomat");
    admin_admin.setMOS_User(&admin_mos);
    admin_admin.save();

    Project metalab;
    metalab.setAdmin_password("kassomat");
    metalab.setMOS_User(&admin_mos);
    metalab.setValid_until(QDateTime(QDate(2099,12,31)));
    metalab.setDescription("Fuer Getraenke sowie Abrechnung fuer diverse Bereitstellungen von Infrastruktur (Laser, Vinylcutter, Drucker,..)");
    metalab.setFunding_target_max(0);
    metalab.setFunding_target_min(0);
    metalab.setImage_url("metalab.png");
    metalab.setName("Metalab");
    metalab.setNotification("alexander-sulz@gmx.at");
    metalab.save();

    Product mate;
    mate.setBarcode("01101011");
    mate.setCost(200);
    mate.setDisplay_name("Mate");
    mate.setImage_url("mate.png");
    mate.setProject(&metalab);
    mate.save();

    Product mischmasch;
    mischmasch.setBarcode("01111011");
    mischmasch.setCost(150);
    mischmasch.setDisplay_name("Mischmasch");
    mischmasch.setImage_url("mischmasch.png");
    mischmasch.setProject(&metalab);
    mischmasch.save();
}

void DatabaseController::dbTestCleanup(){
    QDjangoQuerySet<Log>().remove();
    QDjangoQuerySet<Moneycode>().remove();
    QDjangoQuerySet<Booking_Row>().remove();
    QDjangoQuerySet<Booking>().remove();
    QDjangoQuerySet<Product>().remove();
    QDjangoQuerySet<Project>().remove();
    QDjangoQuerySet<Admin>().remove();
    QDjangoQuerySet<MOS_User>().remove();
    QDjangoQuerySet<User>().remove();

    QDjango::registerModel<Log>().dropTable();
    QDjango::registerModel<Moneycode>().dropTable();
    QDjango::registerModel<Booking_Row>().dropTable();
    QDjango::registerModel<Booking>().dropTable();
    QDjango::registerModel<Product>().dropTable();
    QDjango::registerModel<Project>().dropTable();
    QDjango::registerModel<Admin>().dropTable();
    QDjango::registerModel<MOS_User>().dropTable();
    QDjango::registerModel<User>().dropTable();
}

QDjangoQuerySet<Project> DatabaseController::listProjects(bool active=false){
    QDjangoQuerySet<Project> projects;

    if(active)
        projects = projects.filter( QDjangoWhere("valid_until", QDjangoWhere::GreaterOrEquals, QDateTime::currentDateTime()) );

    return projects;
}


QDjangoQuerySet<Project> DatabaseController::listProjects(QDateTime from, QDateTime to=QDateTime::currentDateTime()){
    QDjangoQuerySet<Project> projects;

    projects = projects.filter( QDjangoWhere("start_date", QDjangoWhere::GreaterOrEquals, from) &&
                                QDjangoWhere("start_date", QDjangoWhere::LessOrEquals, to) );

    return projects;
}

QDjangoQuerySet<Project> DatabaseController::listProjects(Product product){
    QDjangoQuerySet<Project> projects;

    //projects = projects.filter( QDjangoWhere("id", QDjangoWhere::Equals, product.project()->id()) );

    return projects;
}

// --------------------------------------------------------------------------------------------

QDjangoQuerySet<Product> DatabaseController::listProducts(){
    QDjangoQuerySet<Product> products;

    return products;
}

QDjangoQuerySet<Product> DatabaseController::listProducts(Project project){
    QDjangoQuerySet<Product> some_products;
    //some_products= some_products.filter( QDjangoWhere("project_id", QDjangoWhere::Equals, project.id()) );

    return some_products;
}

QDjangoQuerySet<Product> DatabaseController::listProducts(int min_price, int max_price=-1){
    QDjangoQuerySet<Product> some_products;
    QDjangoWhere query = QDjangoWhere("price", QDjangoWhere::GreaterOrEquals, min_price);

    if(max_price != -1)
        query = query && QDjangoWhere("price", QDjangoWhere::LessOrEquals, max_price);

    some_products = some_products.filter( query );

    return some_products;
}

QDjangoQuerySet<Product> DatabaseController::listProducts(Project project, int min_price, int max_price=-1){
    QDjangoQuerySet<Product> some_products;

    //some_products = this->listProducts(min_price, max_price).filter( QDjangoWhere("project_id", QDjangoWhere::Equals, project.id()) );

    return some_products;
}

//GenericModel<Product> DatabaseController::listProductsFoo(Project p, int min_price, int max_price=-1){
//    QDjangoQuerySet<Product> some_products;
//    GenericModel<Product> pi = new GenericModel<Product>();

//    some_products = this->listProducts(min_price, max_price).filter( QDjangoWhere("project_id", QDjangoWhere::Equals, p.id()) );

//    //GenericModel<Product> productlist(some_products);

//    //return GenericModel<Product>(some_products);
//    return pi;
//}
