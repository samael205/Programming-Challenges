#include "mainwindow.h"
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow()
    : savedTasksPath("myToDo"){
    ToDoList = new TaskWidget(this);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    notes = new QTextEdit;
    notes->setReadOnly(true);
    notes->setFixedHeight(100);
    notes->setPlaceholderText(tr("Description"));

    mainLayout->addWidget(ToDoList);

    QLabel *notesLabel = new QLabel("Task notes");
    mainLayout->addWidget(notesLabel);
    mainLayout->addWidget(notes);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainLayout);

    SetupMenu();
    Restore();

    connect(ToDoList, SIGNAL(selected()), this, SLOT(showTaskNote()));

    QRect desktop = QApplication::desktop()->screenGeometry();
    int x = desktop.width();
    int y = desktop.height();

    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    animation->setEndValue(QRect(x/2-300, y/2-300, 600, 500));
    animation->start();
}

MainWindow::~MainWindow(){
    ToDoList->saveToFile(savedTasksPath);
}

void MainWindow::Restore(){
    QFile file(savedTasksPath);
    if(file.exists())
        ToDoList->readFromFile(savedTasksPath);
}

void MainWindow::SetupMenu(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    saveToFile = new QAction("Save", this);
    saveToFile->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveToFile);
    connect(saveToFile, SIGNAL(triggered(bool)), this, SLOT(save()));

    readFromFile = new QAction("Load", this);
    readFromFile->setShortcut(QKeySequence::Open);
    fileMenu->addAction(readFromFile);
    connect(readFromFile, SIGNAL(triggered(bool)), this, SLOT(read()));

    fileMenu->addSeparator();

    exit = new QAction(tr("Exit"), this);
    exit->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exit);
    connect(exit, SIGNAL(triggered(bool)), this, SLOT(exitProgram()));

    taskMenu = menuBar()->addMenu(tr("&Task"));
    addTask = new QAction("New", this);
    addTask->setShortcut(QKeySequence::New);
    taskMenu->addAction(addTask);
    connect(addTask, SIGNAL(triggered(bool)), ToDoList, SLOT(showAddTaskDialog()));

    editTask = new QAction("Edit", this);
    editTask->setShortcut(tr("CTRL+E"));
    taskMenu->addAction(editTask);
    connect(editTask, SIGNAL(triggered(bool)), ToDoList, SLOT(editTask()));

    removeTask = new QAction("Remove", this);
    removeTask->setShortcut(QKeySequence::Delete);
    taskMenu->addAction(removeTask);
    connect(removeTask, SIGNAL(triggered(bool)), ToDoList, SLOT(removeTask()));

    clearAll = new QAction("Remove All", this);
    taskMenu->addAction(clearAll);
    clearAll->setShortcut(tr("CTRL+DEL"));
    connect(clearAll, SIGNAL(triggered(bool)), ToDoList, SLOT(clearAll()));
}

void MainWindow::save(){
    QString file = QFileDialog::getSaveFileName(this);
    if(!file.isEmpty()){
        ToDoList->saveToFile(file);
        statusBar()->showMessage("Saved");
    }
}

void MainWindow::read(){
    QString file = QFileDialog::getOpenFileName(this);
    if(!file.isEmpty()){
        ToDoList->readFromFile(file);
        statusBar()->showMessage(tr("Opened %1").arg(file));
    }
}

void MainWindow::showTaskNote(){
    QString desc = ToDoList->taskDescription();
    notes->setText(ToDoList->taskDescription());
}

void MainWindow::exitProgram(){
    QGuiApplication::exit();
}

