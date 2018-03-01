#include "client.h"
#include "usernamedialog.h"

Client::Client(){   
    usernameDialog setMyNickname;

    if(setMyNickname.exec()){
        username = setMyNickname.username->text();

        manager = new ConnectionManager(username, this);
        manager->setPort(server.serverPort());
        manager->startTransmiting();
   }

    if(username.isEmpty())
        username = tr("unknown");

   connect(&server, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
   connect(manager, SIGNAL(newConnection(Connection*)), this, SLOT(newConnection(Connection*)));
}

bool Client::isConnected(QHostAddress hostIp){
    if(clients.contains(hostIp))
        return true;
    return false;
}

void Client::hostIsReady(){
    Connection * connection = qobject_cast<Connection*>(sender());
    if(isConnected(connection->peerAddress()))
        return;

    clients.insert(connection->peerAddress(), connection);
    emit newUser(connection->clientNick());
}

void Client::sendMessage(const QString & message){
    if(message.isEmpty())
        return;

    QList<Connection*> hosts = clients.values();

    foreach(Connection * connection, hosts)
        connection->sendMessage(username + "@" + message);
}

void Client::newConnection(Connection * connection){
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyRead()), this, SLOT(hostIsReady()));
    connect(connection, SIGNAL(newMessage(QString,QString)), this, SIGNAL(newMessage(QString,QString)));

    if(connection->isOpen()){
        QString sendReady = "Chat@Ready";
        connection->sendMessage(sendReady);
    }
}

void Client::disconnected(){
    if(Connection * connection = qobject_cast<Connection*>(sender()))
        removeConnection(connection);
}

void Client::removeConnection(Connection * connection){
    if(clients.contains(connection->peerAddress())){
        clients.remove(connection->peerAddress());
        QString nick = connection->clientNick();
        if(!nick.isEmpty())
            emit userLeft(nick);
    }
    connection->deleteLater();
}


