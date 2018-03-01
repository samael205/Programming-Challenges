#ifndef CLIENT_H
#define CLIENT_H

#include "server.h"
#include "connection.h"
#include "connectionmanager.h"

#include <QDebug>
#include <QMultiHash>

class ConnectionManager;

class Client : public QObject{
    Q_OBJECT
public:
    Client();

    bool isConnected(QHostAddress);
    void sendMessage(const QString&);
signals:
    void newMessage(const QString&, const QString&);
    void newUser(const QString&);
    void userLeft(const QString&);
public slots:
    void newConnection(Connection*);
    void disconnected();
    void hostIsReady();
private:
    void removeConnection(Connection*);

    QString username;
    ConnectionManager * manager = nullptr;
    QMultiHash<QHostAddress, Connection*> clients;
    Server server;
};

#endif // CLIENT_H
