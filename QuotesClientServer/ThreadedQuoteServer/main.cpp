#include <QApplication>
#include "dialog.h"

int main(int argv, char ** argc){
    QApplication app(argv, argc);

    Dialog serverDialog;
    serverDialog.show();

    return app.exec();
}
