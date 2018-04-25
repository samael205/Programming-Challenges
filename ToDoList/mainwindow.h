#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include "taskwidget.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    const QString savedTasksPath;
    MainWindow();
    ~MainWindow();
private slots:
     void save();
     void read();
     void showTaskNote();
     void exitProgram();
private:
    QTextEdit * notes = nullptr;
    TaskWidget * ToDoList = nullptr;

    void SetupMenu();


    QMenu * taskMenu = nullptr;
    QMenu * fileMenu = nullptr;
    QAction * editTask = nullptr;
    QAction * removeTask = nullptr;
    QAction * clearAll = nullptr;
    QAction * saveToFile = nullptr;
    QAction * readFromFile = nullptr;
    QAction * addTask = nullptr;

    QAction * exit = nullptr;
protected:
    void Restore();
};

#endif // MAINWINDOW_H
