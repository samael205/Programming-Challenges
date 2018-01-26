#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "taskwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();
private slots:
     void save();
     void read();
private:
    TaskWidget * ToDoList;

    void SetupMenu();

    static QString path;

    QMenu * taskMenu;
    QMenu * fileMenu;
    QAction * editTask;
    QAction * removeTask;
    QAction * clearAll;
    QAction * saveToFile;
    QAction * readFromFile;
    QAction * addTask;
protected:
    void Restore();
};

#endif // MAINWINDOW_H