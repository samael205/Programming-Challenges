#ifndef SCANINFOMODEL_H
#define SCANINFOMODEL_H

#include <QAbstractTableModel>

#define REP(i, n) for(int i=0; i<n; i++)

struct info{
    int port;
    QString status;
    QString description;
};

class ScanInfoModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ScanInfoModel(QObject* parent = 0);
    ScanInfoModel(const QString &, QObject* parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    QList<info> scanInfo;

};

#endif // SCANINFOMODEL_H
