#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include "qtquick2applicationviewer.h"
#include "kassomatcontroller.h"
#include "databasecontroller.h"
#include "db/QDjango.h"
#include "db/QDjangoQuerySet.h"
#include "product.h"
#include "genericmodel.h"
#include "log/QsLog.h"
#include "DestSQL.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    KassomatController kassomatController;
    DatabaseController databaseController;

    QsLogging::Logger& logger = QsLogging::Logger::instance();
    //logger.setLoggingLevel(QsLogging::TraceLevel);
    QsLogging::DestinationPtr destSQL( QsLogging::DestinationPtr(new DestSQL) );
    logger.addDestination(destSQL);

    //QList<Product *> p;
    GenericModel<Project> projectlist(databaseController.listProjects(false));

    //p = databaseController.listProducts();
    //productlist.addItems(p);

    //kassomatController.setSmartPayoutDevice("/dev/ttyACM0");

    qmlRegisterType<Product>("db.product",1,0,"productlist");

//    QtQuick2ApplicationViewer viewer;
//    //in .qml files eine variable namens "controller" global verfuegbar machen

//    viewer.rootContext()->setContextProperty("controller", &kassomatController);
//    viewer.rootContext()->setContextProperty("projectlist", &projectlist );
//    viewer.rootContext()->setContextProperty("database", &databaseController );
//    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));
//    viewer.showExpanded();

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl::fromLocalFile("./qml/kassomat/main.qml"));

    engine.rootContext()->setContextProperty("controller", &kassomatController);
    engine.rootContext()->setContextProperty("projectlist", &projectlist );
    engine.rootContext()->setContextProperty("database", &databaseController );


    component.create();

    return app.exec();
}
