#ifndef GALLERY_H
#define GALLERY_H

#include <QtWidgets>
#include "thumbnail_item.h"

class Gallery : public QWidget
{
    Q_OBJECT
public:
    Gallery(QWidget * parent = 0);
private:
    QListWidget * icons;
};

#endif // GALLERY_H
