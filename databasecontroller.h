#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QtSql>
#include "project.h"
#include "product.h"
#include "user.h"
#include "moneycode.h"
#include "db/QDjango.h"
#include "db/QDjangoQuerySet.h"
#include "genericmodel.h"

class DatabaseController : public QObject{

    Q_OBJECT

public:
    DatabaseController();

    void dbTestFill();

    bool newProject();
    bool shutdownProject(Project project); // sends all MOS users their redeem code again, set valid_until to hide the project (if not set)
    bool finishProject(Project project); // payout the money & email MOS users, success!,  set valid_until to hide the project (if not set)

    bool newProduct(Project project);
    bool deleteProduct(Product product); // generate moneycodes and/or send updates

    bool newUser(QString email);

    bool addMoneycode(User user); //generates and adds a moneycode to a user
    bool sendMoneycode(User user); //sends an email about all the open moneycode for this user
    bool redeemMoneycode(User user, QString code); //payout that moneycode

    QDjangoQuerySet<Project> listProjects(bool active);
    QDjangoQuerySet<Project> listProjects(QDateTime from, QDateTime to);
    QDjangoQuerySet<Project> listProjects(Product product);

    QDjangoQuerySet<Product> listProducts(); // QList<Product*>  test function, .. making this work.. damnit
    QDjangoQuerySet<Product> listProducts(Project project);
    QDjangoQuerySet<Product> listProducts(int min_price, int max_price);
    QDjangoQuerySet<Product> listProducts(Project project, int min_price, int max_price);
    //GenericModel<Product> listProductsFoo(Project p, int min_price, int max_price);

    bool buyProduct(Product p);
    //
    // allen usern fuer abgelaufenes projekt nen gutschrift code ausstellen und per mail schicken
    // gutscheincode erneut senden lassen. (neu generieren).
    // gutschrift code einloesen.
    //
    //

public slots:
    void dbTestCleanup();


private:
    QSqlDatabase m_db;
};

#endif // DATABASECONTROLLER_H
