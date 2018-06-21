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

    const auto & task = tasks.at(index.row());
    if(role == Qt::CheckStateRole)
        return task.progress->checkState();
    else if(role == Qt::DisplayRole)
        return task.progress->text();
    else if(role == Qt::EditRole)
        return task.description;

    return QVariant();
}

bool TaskModel::insertRows(int row, int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    REP(i, count)
            tasks.insert(row, {new QCheckBox(), QString()});
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
        task.progress->setText(value.toString());
        task.progress->setTristate(false);
        tasks.replace(index.row(), task);
        emit(dataChanged(index, index));
        return true;
    }
    else if(index.isValid() && role == Qt::CheckStateRole){
        auto task = tasks.value(index.row());
        task.progress->setChecked(value.toBool());
        tasks.replace(index.row(), task);
        emit(dataChanged(index, index));
        return true;
    }
    else if(index.isValid() && role == Qt::EditRole){
        auto task = tasks.value(index.row());
        task.description = value.toString();
        tasks.replace(index.row(), task);
        emit(dataChanged(index, index));
        return true;
    }
    return false;
}

QString TaskModel::showNote(QModelIndex & index, int role){
    if(index.isValid() && role == Qt::DisplayRole){
        const auto & task = tasks.value(index.row());
        return task.description;
    }
    return QString();
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const{   
    if(!index.isValid())
        return Qt::ItemIsEditable;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool TaskModel::containCheckBox(QString nameTask){
    foreach (auto task, tasks)
       if(task.progress->text() == nameTask)
           return true;
    return false;
}

QList<Task> TaskModel::getTasks() const{
    return tasks;
}
