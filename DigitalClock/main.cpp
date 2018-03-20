#include <QApplication>
#include <QIcon>

#include "digitalshow.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);

    QString iconName("icons/clock.png");
    QIcon appIcon(iconName);
    app.setWindowIcon(appIcon);

    DigitalShow clock;
    clock.show();

    return app.exec();
}
