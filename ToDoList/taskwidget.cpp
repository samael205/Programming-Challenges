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

TaskWidget::TaskWidget(QWidget * parent)
    : QTabWidget(parent){
    taskList = new TaskModel(this);
    setFixedWidth(600);
    Setup();
}

void TaskWidget::Setup(){
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(taskList);

    QTableView * viewModel = new QTableView;
    viewModel->setModel(proxy);
    viewModel->setSortingEnabled(true);
    viewModel->setFixedWidth(width());
    viewModel->setColumnWidth(0, width()-20);

    QString iconPath(QDir::currentPath() + "/icons/progress.png");
    QIcon tabIcon(iconPath);

    addTab(viewModel, tabIcon, "Productivity");

}

void TaskWidget::addTask(QString taskDescription, bool status){
    if(!taskList->containCheckBox(taskDescription)){
        taskList->insertRows(0, 1, QModelIndex());
        QModelIndex index = taskList->index(0, 0, QModelIndex());
        taskList->setData(index, taskDescription, Qt::DisplayRole);
        taskList->setData(index, status, Qt::CheckStateRole);
    }
    else
        QMessageBox::information(this, tr("Duplicate Name"),
                                 tr("The name %1 already exists.").arg(taskDescription));

}

void TaskWidget::showAddTaskDialog(){
    addTaskDialog addNewTask;

    if(addNewTask.exec()){
        QString task = addNewTask.taskName->text();
        addTask(task);
    }
}

void TaskWidget::editTask(){
    QTableView * view = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel * proxy = static_cast<QSortFilterProxyModel*>(view->model());
    QItemSelectionModel * selectModel = view->selectionModel();

    QModelIndexList indexes = selectModel->selectedRows();
    QString description;
    int row;

    foreach(QModelIndex index, indexes){
        row = proxy->mapToSource(index).row();
        QModelIndex nameIndex = taskList->index(row, 0, QModelIndex());
        QVariant taskName = taskList->data(nameIndex, Qt::DisplayRole);
        description = taskName.toString();

        addTaskDialog aDialog;
        aDialog.setWindowTitle("Edit Task");

       aDialog.taskName->setReadOnly(false);
        aDialog.taskName->setText(description);

        if(aDialog.exec()){
            QString newTask = aDialog.taskName->text();
            QModelIndex index = taskList->index(row, 0, QModelIndex());
             taskList->setData(index, newTask, Qt::DisplayRole);
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

void TaskWidget::clearAll(){
   taskList->removeRows(0, taskList->getTasks().size(), QModelIndex());
}

std::string TaskWidget::taskStatus(Qt::CheckState status){
    if(status == Qt::Checked)
        return "DONE";
    return "UNDONE";
}

int Status(std::string check){
    if(check == "DONE")
        return 1;
    return 0;
}

void TaskWidget::saveToFile(QString & fileName){
    std::ofstream file;
    file.open(fileName.toStdString());

    if(!file.is_open()){
        QMessageBox::information(this, tr("Could not open file."), tr("Can't write content to file!"));
        return;
    }

    for(const auto task : taskList->getTasks())
        file<<taskStatus(task->checkState())<<";"
           <<task->text().toStdString()<<"\n";

    file.close();
}

void TaskWidget::readFromFile(QString & fileName){
    std::ifstream file;
    file.open(fileName.toStdString());
    if(!file.is_open())
        QMessageBox::information(this, tr("Could not open file."),
                                 tr("Can't read content from file!"));

    std::vector<std::pair<std::string, std::string>> task;
    std::vector<std::string> getInfo;
    std::string temp, bufor;

    while(std::getline(file, temp)){
        std::stringstream sstream(temp);
        while(std::getline(sstream, bufor, ';'))
            getInfo.push_back(bufor);
        sstream.str("");
        task.push_back(std::pair<std::string, std::string>(getInfo.front(), getInfo.back()));
        getInfo.clear();
    }
    file.close();

    for(auto & each : task)
        addTask(QString::fromStdString(each.second), Status(each.first));
}
