TEMPLATE = app

QT += qml quick xml
CONFIG += c++11

SOURCES += main.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

QML_DESIGNER_IMPORT_PATH =


DEFINES += QT_DEPRECATED_WARNINGS

OTHER_FILES = main.qml \
              subcontent/*.qml \
              subcontent/icons/*

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
