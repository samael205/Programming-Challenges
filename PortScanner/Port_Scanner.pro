TEMPLATE = app

CONFIG += c++11

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window

SOURCES += \
    scanner.cpp \
    main.cpp \
    scaninfomodel.cpp \
    mainwindow.cpp \
    scanstatusdelegate.cpp

HEADERS += \
    scanner.h \
    scaninfomodel.h \
    mainwindow.h \
    scanstatusdelegate.h

RESOURCES += \
    content.qrc

