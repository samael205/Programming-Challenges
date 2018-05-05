#include "mainwindow.h"

#include <QPropertyAnimation>
#include <QGuiApplication>

MainWindow::MainWindow(){
    portScanner = new Scanner;

    setCentralWidget(portScanner);

    setupMenus();

    QPropertyAnimation * startupAnimation;
    startupAnimation = new QPropertyAnimation(this, "size");
    QSize size(600, 600);
    startupAnimation->setEndValue(size);
    startupAnimation->start();

    connect(portScanner, SIGNAL(start()), this, SLOT(ResizeWindowToSeeResultsContent()));
}

void MainWindow::setupMenus(){
    fileMenu = menuBar()->addMenu("File");

    saveToCSV = new QAction("Save", this);
    saveToCSV->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveToCSV);
    connect(saveToCSV, SIGNAL(triggered(bool)), portScanner, SLOT(saveResultsToCSV()));

    fileMenu->addSeparator();

    exit = new QAction("Exit", this);
    exit->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exit);
    connect(exit, SIGNAL(triggered(bool)), this, SLOT(quit()));
}

void MainWindow::quit(){
    QGuiApplication::exit();
}

void MainWindow::ResizeWindowToSeeResultsContent(){
    if(width() < 1000 || height() < 700){
        QPropertyAnimation * animation;
        animation = new QPropertyAnimation(this, "size");
        QSize resizeWindow(1000, 700);
        animation->setEndValue(resizeWindow);
        animation->start();
    }
}
