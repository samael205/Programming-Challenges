#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtNetwork/QTcpSocket>

class Connection : public QTcpSocket{
    Q_OBJECT
public:
    Connection(QString nick = "unknown", QObject * parent = nullptr);

    QString clientNick() const;
    bool sendMessage(const QString&);
signals:
    void newMessage(const QString&, const QString&);
public slots:
    void readData();
private:
    QString username;
};

#endif // CONNECTION_H
