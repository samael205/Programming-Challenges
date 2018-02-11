QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(printsupport): QT += printsupport

TARGET = Notepad
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        notepad.cpp \
        hightlight.cpp

HEADERS  += notepad.h \
    hightlight.h

FORMS    += notepad.ui

DISTFILES +=

