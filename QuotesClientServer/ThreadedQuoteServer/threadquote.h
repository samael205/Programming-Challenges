#ifndef THREADQUOTE_H
#define THREADQUOTE_H

#include <QtNetwork/QTcpSocket>
#include <QThread>

class ThreadQuote : public QThread
{
    Q_OBJECT
public:
    ThreadQuote(int, const QString& ,QObject * parent = 0);

    void run() override;
signals:
    void error(QTcpSocket::SocketError socketError);
private:
    QString quoteContent;
    int socketDescriptor;
};

#endif // THREADQUOTE_H
