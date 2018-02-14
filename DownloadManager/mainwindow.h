#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "downloaderwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

public slots:
    void startDownload();
    void quit();
private:
    downloaderWidget * downloader;

    void setup();
    QMenu * downloadMenu;
    QAction * addDownload;
    QAction * remove;
    QAction * exitFromProgram;
};

#endif // MAINWINDOW_H
