#include "taskwidget.h"
#include "addtaskdialog.h"

#include <QTableView>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QFile>
#include <QDataStream>
#include <QTextCodec>
#include <QDebug>
#include <QRegExp>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>

TaskWidget::TaskWidget(QWidget * parent)
    : QTabWidget(parent){
    taskList = new TaskModel(this);

    Setup();
}

void TaskWidget::Setup(){

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(taskList);

    QTableView * viewModel = new QTableView;
    viewModel->setModel(proxy);
    viewModel->setSortingEnabled(true);
    viewModel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(viewModel->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectChanged()));

    QString iconPath(QDir::currentPath() + "/icons/progress.png");
    QIcon tabIcon(iconPath);

    addTab(viewModel, tabIcon, "Productivity");

}

void TaskWidget::addTask(QString taskDescription, QString note, bool status){

    if(!taskList->containCheckBox(taskDescription)){
        if(note.isEmpty())
            note = "NONE";
        taskList->insertRows(0, 1, QModelIndex());
        QModelIndex index = taskList->index(0, 0, QModelIndex());
        taskList->setData(index, taskDescription, Qt::DisplayRole);
        taskList->setData(index, note, Qt::EditRole);
        taskList->setData(index, status, Qt::CheckStateRole);
    }
    else
        QMessageBox::information(this, "Duplicate Name",
                                 tr("The name %1 already exists.").arg(taskDescription));
}

void TaskWidget::showAddTaskDialog(){

    addTaskDialog addNewTask;

    if(addNewTask.exec()){
        QString task = addNewTask.taskName->text();
        QString note = addNewTask.noteName->toPlainText();
        addTask(task, note);
    }
}

void TaskWidget::editTask(){

    QTableView * view = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel * proxy = static_cast<QSortFilterProxyModel*>(view->model());
    QItemSelectionModel * selectModel = view->selectionModel();

    QModelIndexList indexes = selectModel->selectedRows();
    QString description;
    QString note;
    int row;

    foreach(QModelIndex index, indexes){
        row = proxy->mapToSource(index).row();
        QModelIndex nameIndex = taskList->index(row, 0, QModelIndex());
        QVariant taskName = taskList->data(nameIndex, Qt::DisplayRole);
        QVariant noteName = taskList->data(nameIndex, Qt::EditRole);
        description = taskName.toString();
        note = noteName.toString();
        if(note == "NONE")
            note.clear();

        addTaskDialog aDialog;
        aDialog.setWindowTitle("Edit Task");

        aDialog.taskName->setReadOnly(false);
        aDialog.taskName->setText(description);
        aDialog.noteName->setText(note);

        if(aDialog.exec()){
            QString newTask = aDialog.taskName->text();
            QString newNote = aDialog.noteName->toPlainText();
            QModelIndex index = taskList->index(row, 0, QModelIndex());
             taskList->setData(index, newTask, Qt::DisplayRole);
             taskList->setData(index, newNote, Qt::EditRole);
        }
    }
}

void TaskWidget::removeTask(){
        QTableView * view = static_cast<QTableView*>(currentWidget());
        QSortFilterProxyModel * proxy = static_cast<QSortFilterProxyModel*>(view->model());
        QItemSelectionModel * selectModel = view->selectionModel();

        QModelIndexList indexes = selectModel->selectedRows();

        foreach(QModelIndex index, indexes){
            int row = proxy->mapToSource(index).row();
            taskList->removeRows(row, 1, QModelIndex());
        }
}

const QString TaskWidget::taskDescription() const{
    QTableView * view = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel * proxy = static_cast<QSortFilterProxyModel*>(view->model());
    QItemSelectionModel * select = view->selectionModel();

    QModelIndex index = select->currentIndex();
    int row = proxy->mapToSource(index).row();
    QModelIndex noteIndex = taskList->index(row, 0, QModelIndex());

    return taskList->showNote(noteIndex, Qt::DisplayRole);
}

void TaskWidget::selectChanged(){
    emit selected();
}

void TaskWidget::clearAll(){
   taskList->removeRows(0, taskList->getTasks().size(), QModelIndex());
}

void TaskWidget::saveToFile(const QString & fileName){
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, "File Error", tr("Failed to open file %1")
                                 .arg(fileName));
        return;
    }

    int n, m;
    n = taskList->rowCount(QModelIndex());
    m = taskList->columnCount(QModelIndex());
    QDataStream data(&file);
    data << n << m;
    QModelIndex index;

    REP(i, n)
            REP(j, m){
                index = taskList->index(i, j, QModelIndex());
                data << taskList->data(index, Qt::CheckStateRole);
                data << taskList->data(index, Qt::DisplayRole);
                data << taskList->data(index, Qt::EditRole);
    }

    file.close();

}

void TaskWidget::readFromFile(const QString & fileName){
   QFile file(fileName);
   if(!file.open(QIODevice::ReadOnly)){
       QMessageBox::information(this, "File error", tr("Failed to open file %1")
                                .arg(fileName));
       return;
   }
   int n, m;
   QDataStream data(&file);
   data >> n >> m;
   QModelIndex index;
   taskList->insertRows(0, n, QModelIndex());
   REP(i, n)
           REP(j, m){
                index = taskList->index(i, j, QModelIndex());
                taskList->setData(index, data, Qt::CheckStateRole);
                taskList->setData(index, data, Qt::DisplayRole);
                taskList->setData(index, data, Qt::EditRole);

   }
   file.close();
}

