#ifndef THREADSERVER_H
#define THREADSERVER_H

#include <QtNetwork/QTcpServer>
#include "threadquote.h"

class ThreadServer : public QTcpServer
{
    Q_OBJECT
public:
    ThreadServer(QObject * parent = 0);
protected:
    void incomingConnection(qintptr) override;
private:
    QStringList quotesList;
};

#endif // THREADSERVER_H
