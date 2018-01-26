#include <QApplication>
#include "mainwindow.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);
    MainWindow productivity;
    productivity.show();
    return app.exec();
}
