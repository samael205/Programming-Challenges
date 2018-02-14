#include <QApplication>
#include "mainwindow.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);

    MainWindow downloadManager;
    downloadManager.show();

    return app.exec();
}
