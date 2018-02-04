#include "mainwindow.h"

#include <QPropertyAnimation>

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

    saveToCSV = new QAction("Export to CSV", this);
    fileMenu->addAction(saveToCSV);
    connect(saveToCSV, SIGNAL(triggered(bool)), portScanner, SLOT(saveResultsToCSV()));
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
