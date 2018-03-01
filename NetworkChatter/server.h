#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>

#include "connection.h"

class Server : public QTcpServer{
    Q_OBJECT
public:
    Server(QObject * parent = nullptr);
signals:
    void newConnection(Connection*);
protected:
    void incomingConnection(qintptr);
};

#endif // SERVER_H
