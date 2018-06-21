#include <QApplication>
#include "imageviewer.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(content);
    QIcon icon(":/icons/icons/eye.png");
    app.setWindowIcon(icon);

    ImageViewer gallery;
    gallery.show();

    return app.exec();
}
