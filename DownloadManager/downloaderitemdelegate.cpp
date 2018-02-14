#include "downloaderitemdelegate.h"
#include <QApplication>

DownloaderItemDelegate::DownloaderItemDelegate(QObject * parent)
    : QStyledItemDelegate(parent){

}

void DownloaderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        if(index.column() == 1){
            int progress = index.data().toInt();

            QStyleOptionProgressBar progressbar;
            progressbar.rect = option.rect;
            progressbar.minimum = 0;
            progressbar.maximum = 100;
            progressbar.progress = progress;
            progressbar.text = QString::number(progress) + "%";
            progressbar.textVisible = true;

            QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                               &progressbar, painter);
        }else
            QStyledItemDelegate::paint(painter, option, index);
}
