QT += quick
CONFIG += c++11

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

QT += qml quick network
CONFIG += c++11

SOURCES += \
        main.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

QML_DESIGNER_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
