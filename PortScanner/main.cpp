#include <QApplication>
#include "mainwindow.h"


int main(int argc, char ** argv){
    QApplication app(argc, argv);
    QIcon icon("./icons/discover.png");
    app.setWindowIcon(icon);

    MainWindow scanner;
    scanner.show();

    app.exec();
}
