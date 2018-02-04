#ifndef THUMBNAIL_ITEM_H
#define THUMBNAIL_ITEM_H

#include <QListWidgetItem>

class Thumbnail_item : public QListWidgetItem{
    using super = QListWidgetItem;
public:
    explicit Thumbnail_item(const QString&);
    virtual QVariant data(int role) const override;
};

#endif // THUMBNAIL_ITEM_H
