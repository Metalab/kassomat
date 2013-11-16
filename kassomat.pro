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
    kassomatcontroller.cpp \
	sspcoms.cpp \
	sspcomstask.cpp \
    databasecontroller.cpp \
    db/QDjangoWhere.cpp \
    db/QDjangoQuerySet.cpp \
    db/QDjangoModel.cpp \
    db/QDjangoMetaModel.cpp \
    db/QDjango.cpp \
    projectcontroller.cpp \
    genericmodelbase.cpp \
    genericmodel.cpp \
    http/QDjangoUrlResolver.cpp \
    http/QDjangoHttpServer.cpp \
    http/QDjangoHttpResponse.cpp \
    http/QDjangoHttpRequest.cpp \
    http/QDjangoHttpController.cpp \
    http/QDjangoFastCgiServer.cpp \
    log/QsLogDestFile.cpp \
    log/QsLogDestConsole.cpp \
    log/QsLogDest.cpp \
    log/QsLog.cpp

HEADERS += \
    smartpayout.h \
    kassomatcontroller.h \
	sspcoms.h \
	sspcomstask.h \
	sspevents.h \
	encryption.h \
    databasecontroller.h \
    db/QDjangoWhere.h \
    db/QDjangoWhere_p.h \
    db/QDjangoQuerySet.h \
    db/QDjangoQuerySet_p.h \
    db/QDjangoModel.h \
    db/QDjangoMetaModel.h \
    db/QDjango.h \
    db/QDjango_p.h \
    admin.h \
    booking_row.h \
    booking.h \
    product.h \
    project.h \
    mos_user.h \
    user.h \
    projectcontroller.h \
    moneycode.h \
    genericmodelbase.h \
    genericmodel.h \
    http/QDjangoUrlResolver.h \
    http/QDjangoHttpServer_p.h \
    http/QDjangoHttpServer.h \
    http/QDjangoHttpResponse_p.h \
    http/QDjangoHttpResponse.h \
    http/QDjangoHttpRequest_p.h \
    http/QDjangoHttpRequest.h \
    http/QDjangoHttpController.h \
    http/QDjangoHttp_p.h \
    http/QDjangoFastCgiServer_p.h \
    http/QDjangoFastCgiServer.h \
    log.h \
    log/QsLogLevel.h \
    log/QsLogDisableForThisFile.h \
    log/QsLogDestFile.h \
    log/QsLogDestConsole.h \
    log/QsLogDest.h \
    log/QsLog.h \
    DestSQL.h

QT += sql serialport

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    http/http.pro \
    db/db.pro \
    src.pro \
    src.pri \
    log/QsLogReadme.txt \
    log/QsLogChanges.txt \
    log/QsLog.pri \
    log/log_example.pro
