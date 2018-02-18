TARGET = ImageViewer

TEMPLATE = app

QT = core gui

qtHaveModule(printsupport): QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    imageviewer.cpp

HEADERS += \
    imageviewer.h
