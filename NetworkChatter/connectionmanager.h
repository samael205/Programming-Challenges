#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include "connection.h"
#include "client.h"

#include <QUdpSocket>
#include <QTimer>

class Client;

class ConnectionManager : public QObject{
    Q_OBJECT
public:
    ConnectionManager(QString, Client * parent = nullptr);

    void setPort(int);
    void startTransmiting();
signals:
    void newConnection(Connection*);
public slots:
    void readData();
    void sendData();
private:
    Client * client = nullptr;
    QUdpSocket transmiter;
    QTimer transmitionTimer;
    QString myUsername;
    int port;
};

#endif // CONNECTIONMANAGER_H
