#include "server.h"

#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkConfiguration>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHostAddress>
#include <QMessageBox>
#include <QTcpSocket>
#include <QVBoxLayout>

Server::Server(QWidget *parent)
    : QDialog(parent),
    status(new QLabel){

    auto quitButton = new QPushButton("exit");
    quitButton->setAutoDefault(false);

    connect(quitButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    QNetworkConfigurationManager manager;
    QNetworkConfiguration config = manager.defaultConfiguration();
    netSession = new QNetworkSession(config, this);
    connect(netSession, SIGNAL(opened()), this, SLOT(sessionOpened()));
    status->setText(tr("Opening network session..."));
    netSession->open();

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch();

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(status);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(server, SIGNAL(newConnection()), this, SLOT(sendQuote()));

    quotes << tr("“Be yourself; everyone else is already taken.” ")
           << tr("“A room without books is like a body without a soul.”")
           << tr("“Without music, life would be a mistake.” ")
           << tr("“I am enough of an artist to draw freely upon my imagination."
                 "\nImagination is more important than knowledge.\nKnowledge is limited."
                 "\nImagination encircles the world.” ")
           << tr("“The difference between genius and stupidity is: genius has its limits.”")
           << tr("“If you're going through hell, keep going.”");

    resize(250, 70);
}

void Server::sessionOpened(){
    server = new QTcpServer(this);

    if(!server->listen()){
        QMessageBox::information(this, "Host Error", tr("Unale to start server %1").arg(server->errorString()));
        return;
    }

    QString ipAddres;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();

    for(int i = 0; i<ipAddressList.size(); i++)
        if(ipAddressList.at(i) != QHostAddress::LocalHost
                    && ipAddressList.at(i).toIPv4Address()){
                ipAddres = ipAddressList.at(i).toString();
                break;
        }

    status->setText(tr("Server is running\nHost IP: %1\nPort: %2")
                    .arg(ipAddres).arg(server->serverPort()));
}

void Server::sendQuote(){
    QByteArray block;
    QDataStream data(&block, QIODevice::WriteOnly);

    data << quotes[qrand()%quotes.size()];

    QTcpSocket * connection = server->nextPendingConnection();
    connect(connection, SIGNAL(disconnected()), connection, SLOT(deleteLater()));

    connection->write(block);
    connection->disconnectFromHost();
}
