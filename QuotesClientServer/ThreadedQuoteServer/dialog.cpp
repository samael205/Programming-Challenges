#include "dialog.h"

#include <QMessageBox>
#include <QNetworkInterface>
#include <QHBoxLayout>
#include <QVBoxLayout>

Dialog::Dialog(QWidget * parent)
    : QWidget(parent){

    status = new QLabel();
    status->setWordWrap(true);

    quit = new QPushButton(tr("quit"));
    quit->setAutoDefault(false);

    if(!server.listen()){
        QMessageBox::critical(this, "Server error",
                              tr("Could not start server %1")
                              .arg(server.errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddresses = QNetworkInterface::allAddresses();

    for(int i = 0; i<ipAddresses.size(); i++)
        if(ipAddresses.at(i) != QHostAddress::LocalHost
                && ipAddresses.at(i).toIPv4Address()){
            ipAddress = ipAddresses.at(i).toString();
            break;
    }

    if(ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    status->setText(tr("Server is running\n IP %1\nPORT %2\n")
                    .arg(ipAddress)
                    .arg(server.serverPort()));

    connect(quit, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout * buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quit);
    buttonLayout->addStretch(1);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(status);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Server"));

    resize(200, 70);
}


