#include "server.h"

Server::Server(QObject * parent)
    : QTcpServer(parent){
    listen(QHostAddress::Any);
}

void Server::incomingConnection(qintptr socket){
    auto connection = new Connection("unknown", this);
    connection->setSocketDescriptor(socket);
    emit newConnection(connection);
}
