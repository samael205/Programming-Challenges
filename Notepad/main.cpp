#include "notepad.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString iconPath("icons/notepad.png");
    QIcon appIcon(iconPath);
    app.setWindowIcon(appIcon);

    Notepad notepad;
    notepad.show();

    return app.exec();
}
