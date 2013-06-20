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
    QList<Product *> p;
    GenericModel<Product> productlist;

    p = databaseController.listProducts();
    productlist.addItems(p);

    //kassomatController.setSmartPayoutDevice("/dev/ttyACM0");

    qmlRegisterType<Product>("db.product",1,0,"productlist");

    QtQuick2ApplicationViewer viewer;
    //in .qml files eine variable namens "controller" global verfuegbar machen

    qDebug() << "cpp: " << productlist.rowCount();
    for( int i=0; i < productlist.rowCount(); i++){
        qDebug() << "productlist[" << i << "]: " << productlist.index(i, 0).data();
    }

    viewer.rootContext()->setContextProperty("controller", &kassomatController);
    viewer.rootContext()->setContextProperty("productlist", &productlist );
    viewer.setMainQmlFile(QStringLiteral("qml/kassomat/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
