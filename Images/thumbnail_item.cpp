#include "thumbnail_item.h"

Thumbnail_item::Thumbnail_item(const QString & fileName)
    : super(fileName){
    setTextAlignment(Qt::AlignCenter);
}

QVariant Thumbnail_item::data(int role) const{
    if(role == Qt::DecorationRole){
        QIcon icon = super::icon();
        if(icon.isNull())
            return QVariant();
        return icon;
    }
    return super::data(role);
}
