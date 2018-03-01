#include "connectionmanager.h"
#include "connection.h"

static const int udpPort = 45000;
static const int transmitionInterval = 2000;

ConnectionManager::ConnectionManager(QString nickname, Client * parent)
    : QObject(parent){
    this->client = parent;
    myUsername = nickname;

    transmiter.bind(QHostAddress::Any, udpPort, QUdpSocket::ShareAddress);
    transmitionTimer.setInterval(transmitionInterval);

    port = 0;

    connect(&transmiter, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&transmitionTimer, SIGNAL(timeout()), this, SLOT(sendData()));
}

void ConnectionManager::setPort(int serverPort){
    port = serverPort;
}
void ConnectionManager::startTransmiting(){
    transmitionTimer.start();
}

void ConnectionManager::sendData(){
    QByteArray datagram(myUsername.toUtf8());
    datagram.append("@");
    datagram.append(QString::number(port).toUtf8());
    transmiter.writeDatagram(datagram, QHostAddress::Broadcast, udpPort);
}

void ConnectionManager::readData(){
    QByteArray datagram;

    while(transmiter.hasPendingDatagrams()){
        datagram.resize(transmiter.pendingDatagramSize());

        QHostAddress senderIp;
        QString senderNickname;
        int senderServerPort;

        transmiter.readDatagram(datagram.data(), datagram.size(),
                                &senderIp);

        auto splittedData = datagram.split('@');

        senderNickname = splittedData.at(0);
        senderServerPort = splittedData.at(1).toInt();

        if(!client->isConnected(senderIp)){
            auto connection = new Connection(senderNickname);
            emit newConnection(connection);
            connection->connectToHost(senderIp, senderServerPort);
        }
    }
}


