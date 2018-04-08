#include <QApplication>
#include "mainwindow.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);
    QIcon icon(QDir::currentPath() + "/icons/todo.png");
    app.setWindowIcon(icon);

    MainWindow productivity;
    productivity.show();

    return app.exec();
}
