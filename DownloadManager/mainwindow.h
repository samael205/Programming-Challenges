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
    downloaderWidget * downloader = nullptr;

    void setup();
    QMenu * downloadMenu = nullptr;
    QAction * addDownload = nullptr;
    QAction * remove = nullptr;
    QAction * exitFromProgram = nullptr;
};

#endif // MAINWINDOW_H
