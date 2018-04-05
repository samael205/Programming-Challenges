QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(printsupport): QT += printsupport

unix:LIBS += -L/usr/local/lib/python2.7 -lpython2.7
CPLUS_INCLUDE_PATH=/usr/include/python2.7
INCLUDEPATH += /usr/include/python2.7

TARGET = Notepad
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        notepad.cpp \
        hightlight.cpp

HEADERS  += notepad.h \
    hightlight.h

FORMS    += notepad.ui

DISTFILES += \
    text_to_audio.py

