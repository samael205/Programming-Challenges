#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include "scanner.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
public slots:
    void ResizeWindowToSeeResultsContent();
private:
    Scanner * portScanner;

    void setupMenus();

    QMenu * fileMenu;
    QAction * saveToCSV;

};

#endif // MAINWINDOW_H
