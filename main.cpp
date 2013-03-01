#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include "qtquick2applicationviewer.h"
#include "kassomatcontroller.h"
#include "databasecontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    KassomatController kassomatController;
    DatabaseController databaseController;

    //kassomatController.setSmartPayoutDevice("/dev/ttyACM0");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));

    viewer.rootContext()->setContextProperty("controller", &kassomatController);
    viewer.showExpanded();

    return app.exec();
}
