#include "connection.h"

Connection::Connection(QString nick, QObject * parent)
    : QTcpSocket(parent){

    username = nick;

    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
}

QString Connection::clientNick() const{
    return username;
}

void Connection::readData(){
    QByteArray data;
    data = readAll();

    auto splittedData(data.split('@'));
    splittedData[0];

    QString username(splittedData[0]);
    QString message( splittedData[1]);

    if(username == "Chat")
        return;

    emit newMessage(username, message);
}

bool Connection::sendMessage(const QString & message){
    if(message.isEmpty())
        return false;

    QByteArray messageToSend(message.toUtf8());
    write(messageToSend);
    return true;
}
