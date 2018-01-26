#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QTabWidget>
#include <QSortFilterProxyModel>
#include <QDir>
#include "taskmodel.h"

class TaskWidget : public QTabWidget
{
    Q_OBJECT
public:
    TaskWidget(QWidget * parent = 0);
    void saveToFile(QString &);
    void readFromFile(QString&);
public slots:
    void showAddTaskDialog();
    void addTask(QString, bool=false);
    void removeTask();
    void editTask();
    void clearAll();
private:
    std::string taskStatus(Qt::CheckState);

    void Setup();

    TaskModel * taskList;
    QSortFilterProxyModel * proxy;
};

#endif // TASKWIDGET_H
