#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>

QString MainWindow::path = QDir::currentPath() + "/myToDo";

MainWindow::MainWindow(){
    ToDoList = new TaskWidget(this);
    setCentralWidget(ToDoList);
    SetupMenu();

    Restore();
}

MainWindow::~MainWindow(){
    ToDoList->saveToFile(path);
}

void MainWindow::Restore(){
    QFile file(path);
    if(file.exists())
        ToDoList->readFromFile(path);
}

void MainWindow::SetupMenu(){

    fileMenu = menuBar()->addMenu(tr("&File"));
    saveToFile = new QAction("Save", this);
    fileMenu->addAction(saveToFile);
    connect(saveToFile, SIGNAL(triggered(bool)), this, SLOT(save()));

    readFromFile = new QAction("Load", this);
    fileMenu->addAction(readFromFile);
    connect(readFromFile, SIGNAL(triggered(bool)), this, SLOT(read()));

    taskMenu = menuBar()->addMenu(tr("&Task"));
    addTask = new QAction("New", this);
    taskMenu->addAction(addTask);
    connect(addTask, SIGNAL(triggered(bool)), ToDoList, SLOT(showAddTaskDialog()));

    editTask = new QAction("Edit", this);
    taskMenu->addAction(editTask);
    connect(editTask, SIGNAL(triggered(bool)), ToDoList, SLOT(editTask()));

    removeTask = new QAction("Remove", this);
    taskMenu->addAction(removeTask);
    connect(removeTask, SIGNAL(triggered(bool)), ToDoList, SLOT(removeTask()));

    clearAll = new QAction("Remove All", this);
    taskMenu->addAction(clearAll);
    connect(clearAll, SIGNAL(triggered(bool)), ToDoList, SLOT(clearAll()));
}

void MainWindow::save(){
    QString file = QFileDialog::getSaveFileName(this);
    if(!file.isEmpty())
        ToDoList->saveToFile(file);
}

void MainWindow::read(){
    QString file = QFileDialog::getOpenFileName(this);
    if(!file.isEmpty())
        ToDoList->readFromFile(file);
}

