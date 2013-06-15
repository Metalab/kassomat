#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QtSql>
#include "project.h"
#include "product.h"
#include "user.h"
#include "moneycode.h"
#include "db/QDjango.h"
#include "db/QDjangoQuerySet.h"


class DatabaseController {


public:
    DatabaseController();

    bool newProject();
    bool shutdownProject(Project project); // sends all MOS users their redeem code again, set valid_until to hide the project (if not set)
    bool finishProject(Project project); // payout the money & email MOS users, success!,  set valid_until to hide the project (if not set)

    bool newProduct(Project project);
    bool deleteProduct(Product product); // generate moneycodes and/or send updates

    bool newUser(QString email);
    bool addMoneycode(User user); //generates and adds a moneycode to a user
    bool sendMoneycode(User user); //sends an email about all the open moneycode for this user
    bool redeemMoneycode(User user, QString code); //payout that moneycode

    QList<Project> listProjects(QDateTime from, QDateTime to=QDateTime::currentDateTime());
    //QList<const Product*> listProjects();
    QList<QVariantMap> listProjects();

    QList<Product*> listProducts(); // test function, .. making this work.. damnit
    QList<Product> listProducts(Project project);

    // new/edit/del projekte
    // (nur leute die member sind duerfen projekte anlegen)
    //
    // new/edit/del produkte fuer ein jeweiliges Projekt
    // new/edit/del user (email, passwort)
    //
    // von einem projekt etwas abheben/schliessen
    // (nur alles auszahlbar wenn ziel erreicht wurde, ansonsten gutscheine falls email, ansonst spende ans lab)
    //
    // etwas kaufen (booking + booking_row anlegen)
    // anonym fuer ein projekt spenden
    //
    // alle projekte in einem zeitrahmen anzeigen
    // alle produkte eines projektes anzeigen (projekt detailansicht)
    //
    // allen usern fuer abgelaufenes projekt nen gutschrift code ausstellen und per mail schicken
    // gutscheincode erneut senden lassen. (neu generieren).
    // gutschrift code einloesen.
    //
    //


private:
    QSqlDatabase m_db;
};

#endif // DATABASECONTROLLER_H
