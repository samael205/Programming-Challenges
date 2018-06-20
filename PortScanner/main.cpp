#include <QApplication>
#include "mainwindow.h"


int main(int argc, char ** argv){
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(content);
    QIcon icon(":/icons/icons/discover.png");
    app.setWindowIcon(icon);

    MainWindow scanner;
    scanner.show();

    app.exec();
}
