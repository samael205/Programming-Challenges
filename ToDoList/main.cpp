#include <QApplication>
#include "mainwindow.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(content);
    QIcon icon(":/icons/icons/todo.png");
    app.setWindowIcon(icon);

    MainWindow productivity;
    productivity.show();

    return app.exec();
}
