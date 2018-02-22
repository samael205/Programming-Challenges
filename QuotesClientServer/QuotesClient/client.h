#ifndef CLIENT_H
#define CLIENT_H

#include <QtNetwork/QTcpSocket>
#include <QLabel>
#include <QtNetwork/QNetworkSession>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDialog>

class Client : public QDialog
{
    Q_OBJECT
public:
    explicit Client(QWidget * parent = nullptr);
private slots:
    void requestNewQuote();
    void sessionOpened();
    void readQuote();
    void enableGetQuote();
private:
    QLineEdit * port = nullptr;
    QComboBox * host = nullptr;
    QLabel * status = nullptr;
    QPushButton * getQuote = nullptr;

    QDataStream read;
    QString currentQuote;
    QTcpSocket * socket = nullptr;

    QNetworkSession * netSession = nullptr;
};

#endif // CLIENT_H
