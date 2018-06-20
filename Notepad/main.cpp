#include "notepad.h"
#include <QApplication>

int main(int argc, char *argv[]){
    setenv("PYTHONPATH", ".", 1);
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(content);

    QString iconPath(":icons/icons/notepad.png");
    QIcon appIcon(iconPath);
    app.setWindowIcon(appIcon);

    Notepad notepad;
    notepad.show();

    return app.exec();
}
