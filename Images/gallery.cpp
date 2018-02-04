#include "gallery.h"

Gallery::Gallery(QWidget * parent)
    : QWidget(parent){
    icons = new QListWidget;
    icons->setViewMode(QListWidget::IconMode);
    icons->setIconSize(QSize(200, 200));
    icons->setResizeMode(QListWidget::Adjust);
    icons->addItem(new Thumbnail_item("haha.png"));
    icons->addItem(new Thumbnail_item("hihi.png"));

   QListView * showIcons;
  // showIcons = new QListView;
  // showIcons->setModel(icons);
}
