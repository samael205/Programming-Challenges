#include <QApplication>
#include "imageviewer.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    QIcon icon(QDir::currentPath() + "/icons/images.png");
    app.setWindowIcon(icon);

    ImageViewer gallery;
    gallery.show();

    return app.exec();
}
