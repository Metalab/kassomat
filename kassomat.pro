# Add more folders to ship with the application, here
folder_01.source = qml/kassomat
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

CONFIG += c++11 debug
LIBS += -lssl -lcrypto

MAC {
        QMAKE_CXXFLAGS += -stdlib=libc++
        QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    smartpayout.cpp \
    sspcoms.cpp \
    sspcomstask.cpp \
    databasecontroller.cpp \
    kassomatcontroller.cpp \
    projectcontroller.cpp \
    QDjango/db/QDjangoWhere.cpp \
    QDjango/db/QDjangoQuerySet.cpp \
    QDjango/db/QDjangoModel.cpp \
    QDjango/db/QDjangoMetaModel.cpp \
    QDjango/db/QDjango.cpp \
    model/genericmodelbase.cpp \
    model/genericmodel.cpp \
    QDjango/http/QDjangoUrlResolver.cpp \
    QDjango/http/QDjangoHttpServer.cpp \
    QDjango/http/QDjangoHttpResponse.cpp \
    QDjango/http/QDjangoHttpRequest.cpp \
    QDjango/http/QDjangoHttpController.cpp \
    QDjango/http/QDjangoFastCgiServer.cpp \
    QsLog/QsLogDestFile.cpp \
    QsLog/QsLogDestConsole.cpp \
    QsLog/QsLogDest.cpp \
    QsLog/QsLog.cpp

HEADERS += \
    smartpayout.h \
    sspcoms.h \
    sspcomstask.h \
    sspevents.h \
    databasecontroller.h \
    projectcontroller.h \
    kassomatcontroller.h \
    QDjango/db/QDjangoWhere.h \
    QDjango/db/QDjangoWhere_p.h \
    QDjango/db/QDjangoQuerySet.h \
    QDjango/db/QDjangoQuerySet_p.h \
    QDjango/db/QDjangoModel.h \
    QDjango/db/QDjangoMetaModel.h \
    QDjango/db/QDjango.h \
    QDjango/db/QDjango_p.h \
    model/admin.h \
    model/booking_row.h \
    model/booking.h \
    model/product.h \
    model/project.h \
    model/mos_user.h \
    model/user.h \
    model/log.h \
    model/moneycode.h \
    model/genericmodelbase.h \
    model/genericmodel.h \
    QDjango/http/QDjangoUrlResolver.h \
    QDjango/http/QDjangoHttpServer_p.h \
    QDjango/http/QDjangoHttpServer.h \
    QDjango/http/QDjangoHttpResponse_p.h \
    QDjango/http/QDjangoHttpResponse.h \
    QDjango/http/QDjangoHttpRequest_p.h \
    QDjango/http/QDjangoHttpRequest.h \
    QDjango/http/QDjangoHttpController.h \
    QDjango/http/QDjangoHttp_p.h \
    QDjango/http/QDjangoFastCgiServer_p.h \
    QDjango/http/QDjangoFastCgiServer.h \
    QsLog/QsLogLevel.h \
    QsLog/QsLogDisableForThisFile.h \
    QsLog/QsLogDestFile.h \
    QsLog/QsLogDestConsole.h \
    QsLog/QsLogDest.h \
    QsLog/QsLog.h \
    qslogdestsql.h

QT += sql serialport

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    QDjango/http/http.pro \
    QDjango/db/db.pro \
    QDjango/src.pro \
    QDjango/src.pri \
    QsLog/QsLogReadme.txt \
    QsLog/QsLogChanges.txt \
    QsLog/QsLog.pri \
    QsLog/log_example.pro
