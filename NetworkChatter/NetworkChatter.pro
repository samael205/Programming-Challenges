QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NetworkChatter
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        chatdialog.cpp \
    client.cpp \
    server.cpp \
    connection.cpp \
    connectionmanager.cpp \
    usernamedialog.cpp

HEADERS  += chatdialog.h \
    client.h \
    server.h \
    connection.h \
    connectionmanager.h \
    usernamedialog.h

FORMS    += chatdialog.ui

RESOURCES += \
    content.qrc
