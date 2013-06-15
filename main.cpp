#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include "qtquick2applicationviewer.h"
#include "kassomatcontroller.h"
#include "databasecontroller.h"
#include "db/QDjango.h"
#include "db/QDjangoQuerySet.h"
#include "product.h"
#include "genericmodel.h"
#include <QtQml>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    KassomatController kassomatController;
    DatabaseController databaseController;
    QList<Product> p;
    GenericModel<Product> *data;

    p = databaseController.listProducts();
    data->addItems(p);

    //kassomatController.setSmartPayoutDevice("/dev/ttyACM0");

    qmlRegisterType<Product>("db.product",1,0,"productlist");

    QtQuick2ApplicationViewer viewer;
    //in .qml files eine variable namens "controller" global verfuegbar machen

    for( int i=0; i < p.size(); i++){
        qDebug() << p.value(i).toString();
    }


    viewer.rootContext()->setContextProperty("controller", &kassomatController);
    viewer.rootContext()->setContextProperty("productlist", QVariant::fromValue(&p) );
    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
