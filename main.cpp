#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include "qtquick2applicationviewer.h"
#include "kassomatcontroller.h"
#include "databasecontroller.h"
#include "QDjango/db/QDjango.h"
#include "QDjango/db/QDjangoQuerySet.h"
#include "model/product.h"
#include "model/genericmodel.h"
#include "QsLog/QsLog.h"
#include "qslogdestsql.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName("Kassomat");
    QCoreApplication::setApplicationVersion("0.0.1");

    QString deviceName = "ttyACM0";
    bool headless = false;

    {
        QCommandLineParser parser;
        parser.setApplicationDescription("Kassomat");
        parser.addHelpOption();
        parser.addVersionOption();

        // Add option to select the device name
        QCommandLineOption deviceNameOption(QStringList() << "n" << "name",
                QCoreApplication::translate("main", "the <name> of the device to connect to."),
                QCoreApplication::translate("main", "device name"));
        parser.addOption(deviceNameOption);

        // Add option to run headless
        QCommandLineOption headlessOption("headless", QCoreApplication::translate("main", "run headless"));
        parser.addOption(headlessOption);

        // Process the actual command line arguments given by the user
        parser.process(app);

        // Evaluate the arguments and set the variables
        if(parser.isSet(deviceNameOption)) {
            deviceName = parser.value(deviceNameOption);
        }

        headless = parser.isSet(headlessOption);
    }

    try {
        // setup the KassomatController
        KassomatController kassomatController;
        kassomatController.setSmartPayoutDevice(deviceName);
        kassomatController.test();

        // setup the DatabaseController
        DatabaseController databaseController;

        // initialize the logging
        QsLogging::Logger& logger = QsLogging::Logger::instance();
        //logger.setLoggingLevel(QsLogging::TraceLevel);
        QsLogging::DestinationPtr destSQL( QsLogging::DestinationPtr(new QsLogDestSQL) );
        logger.addDestination(destSQL);

        //QList<Product *> p;
        GenericModel<Project> projectlist(databaseController.listProjects(false));

        //p = databaseController.listProducts();
        //productlist.addItems(p);

        qmlRegisterType<Product>("db.product",1,0,"productlist");

        //    QtQuick2ApplicationViewer viewer;
        //    //in .qml files eine variable namens "controller" global verfuegbar machen

        //    viewer.rootContext()->setContextProperty("controller", &kassomatController);
        //    viewer.rootContext()->setContextProperty("projectlist", &projectlist );
        //    viewer.rootContext()->setContextProperty("database", &databaseController );
        //    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));
        //    viewer.showExpanded();

        QQmlEngine engine;
        engine.rootContext()->setContextProperty("controller", &kassomatController);
        engine.rootContext()->setContextProperty("projectlist", &projectlist);
        engine.rootContext()->setContextProperty("database", &databaseController);

        if(! headless) {
            QQmlComponent component(&engine, QUrl::fromLocalFile("./qml/kassomat/main.qml"));
            component.create();
        }

        return app.exec();
    } catch(char const *msg) {
        qFatal("FATAL Error: %s", msg);
        return 1;
    }
}
