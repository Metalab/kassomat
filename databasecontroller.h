#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QtSql>
#include "project.h"
#include "product.h"
#include "user.h"

class DatabaseController{


public:
    DatabaseController();

    bool newProject();
    bool shutdownProject(Project project); // allen usern gutscheine verschicken
    bool finishProject(Project project); // email an alle, es war ein erfolg

    bool newProduct(Project project);
    bool deleteProduct(Product product); // generate moneycodes and/or send updates

    bool newUser(QString email);
    bool addMoneycode(User user); //generates and adds a moneycode to a user
    bool sendMoneycode(User user); //sends an email about all the open moneycode for this user
    bool redeemMoneycode(User user, QString moneycode); //paout that moneycode

    QList<Project> listProjects(QDateTime from, QDateTime to=QDateTime::currentDateTime());
    QList<Project> listProjects(User user);
    QList<Product> listProducts(User user);

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
