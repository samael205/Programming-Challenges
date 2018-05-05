#ifndef SCANSTATUSDELEGATE_H
#define SCANSTATUSDELEGATE_H

#include <QStyledItemDelegate>

class scanStatusDelegate : public QStyledItemDelegate{
    Q_OBJECT
public:
    scanStatusDelegate(QObject * parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SCANSTATUSDELEGATE_H
