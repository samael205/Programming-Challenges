#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QNetworkSession>
#include <QLabel>
#include <QDialog>
#include <QVector>
#include <QtGlobal>

class Server : public QDialog
{
    Q_OBJECT
public:
    explicit Server(QWidget * parent = nullptr);
private slots:
    void sessionOpened();
    void sendQuote();
private:
    QLabel * status = nullptr;
    QTcpServer * server = nullptr;
    QNetworkSession * netSession = nullptr;
    QVector<QString> quotes;
};

#endif // SERVER_H
