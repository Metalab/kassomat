#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QtSql>


class DatabaseController {

public:
    DatabaseController();

    // new/edit/del projekte
    // (nur leute die member sind duerfen projekte anlegen)
    // new/edit/del produkte fuer ein jeweiliges Projekt
    // new/edit/del user (email, passwort)
    //
    // von einem projekt etwas abheben/schliessen
    // (nur alles auszahlbar wenn ziel erreicht wurde)
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
