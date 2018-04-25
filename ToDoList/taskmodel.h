#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractTableModel>
#include <QCheckBox>
#include <QVariant>
#include <QtWidgets>
#include <QObject>

#define REP(i, n) for(int i=0; i<n; i++)
#define VAR(i, x) __typeof(x) i = x
#define FOREACH(i, x) for(VAR(i, x.begin()); i != x.end(); i++)

struct Task{
    QCheckBox * progress = nullptr;
    QString description;
};

class TaskModel : public QAbstractTableModel{
    Q_OBJECT
public:
    TaskModel(QObject * parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool setData(const QModelIndex &index, const QVariant &, int role) override;

    QString showNote(QModelIndex&,int);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool containCheckBox(QString);
    QList<Task> getTasks() const;
private:
    QList<Task> tasks;
};

#endif // TASKMODEL_H
