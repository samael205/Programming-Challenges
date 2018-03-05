#include <QApplication>

#include "digitalshow.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);

    DigitalShow timer;
    timer.show();

    return app.exec();
}
