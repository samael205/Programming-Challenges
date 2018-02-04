#include <QApplication>
#include "imageviewer.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);

    ImageViewer gallery;
    gallery.show();

    return app.exec();
}
