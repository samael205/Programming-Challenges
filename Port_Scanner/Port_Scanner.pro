TEMPLATE = app
TARGET = Port_Scanner

CONFIG += c++11

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lsfml-system -lsfml-network -lsfml-graphics -lsfml-window

SOURCES += \
    scanner.cpp \
    main.cpp \
    scaninfomodel.cpp \
    mainwindow.cpp

HEADERS += \
    scanner.h \
    scaninfomodel.h \
    mainwindow.h

