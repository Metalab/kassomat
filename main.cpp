#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include "qtquick2applicationviewer.h"
#include "smartpayout.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    SmartPayout smartPayout;

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));

    viewer.rootContext()->setContextProperty("smartPayout", &smartPayout);
    viewer.showExpanded();

    return app.exec();
}
