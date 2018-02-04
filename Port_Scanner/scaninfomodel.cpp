#include "scaninfomodel.h"

ScanInfoModel::ScanInfoModel(QObject *parent)
    : QAbstractTableModel(parent),
      scanInfo(){
}

ScanInfoModel::ScanInfoModel(const QString & info, QObject *parent)
    : QAbstractTableModel(parent){

}


int ScanInfoModel::columnCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return 3;
}

int ScanInfoModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return scanInfo.size();
}

QVariant ScanInfoModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal){
        switch(section){
            case 0:
                return "Port Number";
            case 1:
                return "Port Status";
            case 2:
                return "Description";
        }
    }
    return QVariant();
}

QVariant ScanInfoModel::data(const QModelIndex &index, int role) const{
    if(index.isValid() && role == Qt::DisplayRole){
        const auto & scanIndex = scanInfo.at(index.row());
        if(index.column() == 0)
            return scanIndex.port;
        if(index.column() == 1)
            return scanIndex.status;
        if(index.column() == 2)
            return scanIndex.description;
    }
    if(role == Qt::TextAlignmentRole)
        switch(index.column()){
            case 0:
            case 1:
            case 2:
                return Qt::AlignCenter;
            default:
                return Qt::AlignLeft;
        }

    return QVariant();
}

bool ScanInfoModel::insertRows(int row, int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), row, row + count - 1);
    REP(i, count)
            scanInfo.insert(row, { int(), QString(), QString() });
    endInsertRows();
    return true;
}

bool ScanInfoModel::removeRows(int row, int count, const QModelIndex &parent){
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    REP(i, count)
            scanInfo.removeAt(row);
    endRemoveRows();
    return true;
}

bool ScanInfoModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(index.isValid() && role == Qt::EditRole){
        int row = index.row();
        auto scanIndexData = scanInfo.value(row);

        if(index.column() == 0)
            scanIndexData.port = value.toInt();
        else if(index.column() == 1)
            scanIndexData.status = value.toString();
        else if(index.column() == 2)
            scanIndexData.description = value.toString();

        scanInfo.replace(row, scanIndexData);
        return true;
    }
    return false;
}

Qt::ItemFlags ScanInfoModel::flags(const QModelIndex &index) const{
    if(index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
