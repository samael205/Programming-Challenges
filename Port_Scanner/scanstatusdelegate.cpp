#include "scanstatusdelegate.h"

#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>

scanStatusDelegate::scanStatusDelegate(QObject * parent)
    : QStyledItemDelegate(parent){

}

void scanStatusDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    if(index.column() == 1){
        QString status = index.data().toString();

        QStyleOptionViewItem text(option);
        text.font.setBold(true);

        if(status == "OPEN"){
            text.palette.setColor(QPalette::Text, Qt::green);
            QStyledItemDelegate::paint(painter, text, index);
        }
        else if(status == "CLOSE"){
            text.palette.setColor(QPalette::Text, Qt::red);
            QStyledItemDelegate::paint(painter, text, index);
        }

    }else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize scanStatusDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const{
    return QSize();
}
