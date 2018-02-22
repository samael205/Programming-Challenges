#include <QApplication>
#include <ctime>
#include "server.h"

int main(int argc, char ** argv){
    QApplication app(argc, argv);
    qsrand(std::time(0));

    Server server;
    server.show();

    return app.exec();
}
