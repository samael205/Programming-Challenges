#include <QApplication>

#include "scanner.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);

    Scanner scanPorts;
    scanPorts.show();

    app.exec();
}
