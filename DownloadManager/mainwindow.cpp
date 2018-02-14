#include "mainwindow.h"

#include <QtWidgets/QMenuBar>
#include <QPropertyAnimation>
#include <QFile>
#include <QGuiApplication>

MainWindow::MainWindow(){
    downloader = new downloaderWidget;
    setCentralWidget(downloader);

    setup();

    QPropertyAnimation * animation;
    animation = new QPropertyAnimation(this, "size");
    animation->setEndValue(QSize(800, 450));
    animation->start();
}

void MainWindow::startDownload(){
    downloader->setDownload();
}

void MainWindow::setup(){
    downloadMenu = menuBar()->addMenu("Download");

    addDownload = new QAction("New", this);
    downloadMenu->addAction(addDownload);
    addDownload->setShortcut(QKeySequence::New);
    connect(addDownload, SIGNAL(triggered(bool)), this, SLOT(startDownload()));

    remove = new QAction("Remove", this);
    downloadMenu->addAction(remove);
    remove->setShortcut(QKeySequence::Delete);
    connect(remove, SIGNAL(triggered(bool)), downloader, SLOT(remove()));

    downloadMenu->addSeparator();

    exitFromProgram = new QAction("Exit", this);
    downloadMenu->addAction(exitFromProgram);
    exitFromProgram->setShortcut(QKeySequence::Quit);
    connect(exitFromProgram, SIGNAL(triggered(bool)), this, SLOT(quit()));
}

void MainWindow::quit(){
    QGuiApplication::exit();
}

