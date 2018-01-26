#include "taskmodel.h"

TaskModel::TaskModel(QObject *parent)
    : QAbstractTableModel(parent){
}

int TaskModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return tasks.size();
}

int TaskModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return 1;
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        switch(section){
            case 0:
                return tr("Task");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::CheckStateRole){
        const auto & status = tasks.at(index.row());
        return status->checkState();
    }
    if(role == Qt::DisplayRole){
        const auto & taskName = tasks.at(index.row());
        return taskName->text();
    }
    return QVariant();
}

bool TaskModel::insertRows(int row, int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    REP(i, count)
            tasks.insert(row, new QCheckBox());
    endInsertRows();
    return true;
}

bool TaskModel::removeRows(int row, int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    REP(i, count)
            tasks.removeAt(row);
    endRemoveRows();
    return true;
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role){  
    if(index.isValid() && role == Qt::DisplayRole){
        auto task = tasks.value(index.row());
        task->setText(value.toString());
        task->setTristate(false);
        tasks.replace(index.row(), task);
        emit(dataChanged(index, index));
        return true;
    }

    if(index.isValid() && role == Qt::CheckStateRole){
        auto task = tasks.value(index.row());
        task->setChecked(value.toBool());
        tasks.replace(index.row(), task);
        return true;
    }
    return false;
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const{   
    if(!index.isValid())
        return Qt::ItemIsEditable;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool TaskModel::containCheckBox(QString nameTask){
    foreach (auto task, tasks)
       if(task->text() == nameTask)
           return true;
    return false;
}

QList<QCheckBox*> TaskModel::getTasks() const{
    return tasks;
}
