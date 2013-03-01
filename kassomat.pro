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

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    smartpayout.cpp \
    kassomatcontroller.cpp \
    itlssp/SSPDownload.c \
    itlssp/SSPComs.c \
    itlssp/ssp_commands.c \
    itlssp/serialfunc.c \
    itlssp/Random.c \
    itlssp/ITLSSPProc.c \
    itlssp/Encryption.c \
    projectlistmodel.cpp \
    databasecontroller.cpp \
    db/QDjangoWhere.cpp \
    db/QDjangoQuerySet.cpp \
    db/QDjangoModel.cpp \
    db/QDjangoMetaModel.cpp \
    db/QDjango.cpp

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qml/kassomat/BubbleButton.qml \
    qml/kassomat/ChangeButton.qml \
    qml/kassomat/Button.qml

HEADERS += \
    smartpayout.h \
    kassomatcontroller.h \
    itlssp/serialfunc.h \
    itlssp/Random.h \
    itlssp/ITLSSPProc.h \
    itlssp/Encryption.h \
    itlssp/defs.h \
    inc/SSPComs.h \
    inc/ssp_defines.h \
    inc/itl_types.h \
    projectlistmodel.h \
    databasecontroller.h \
    db/QDjangoWhere.h \
    db/QDjangoWhere_p.h \
    db/QDjangoQuerySet.h \
    db/QDjangoQuerySet_p.h \
    db/QDjangoModel.h \
    db/QDjangoMetaModel.h \
    db/QDjango.h \
    db/QDjango_p.h \
    projects.h \
    admins.h \
    products.h \
    bookings.h \
    booking_rows.h

CONFIG += debug

QT += sql


