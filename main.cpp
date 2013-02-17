#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include "qtquick2applicationviewer.h"
#include "kassomatcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    KassomatController kassomatController;

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));

    viewer.rootContext()->setContextProperty("controller", &kassomatController);
    viewer.showExpanded();

    return app.exec();
}
