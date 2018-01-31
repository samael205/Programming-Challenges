TEMPLATE = app
TARGET = ToDoList

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    taskmodel.h \
    taskwidget.h \
    addtaskdialog.h \
    mainwindow.h

SOURCES += \
    taskmodel.cpp \
    main.cpp \
    taskwidget.cpp \
    addtaskdialog.cpp \
    mainwindow.cpp
