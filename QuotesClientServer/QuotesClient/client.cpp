#include "client.h"
#include <QtNetwork/QHostInfo>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QPushButton>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QTimer>

Client::Client(QWidget *parent)
    : QDialog(parent),
      socket(new QTcpSocket(this)),
      port(new QLineEdit),
      host(new QComboBox),
      getQuote(new QPushButton(tr("get quote!"))){

    host->setEditable(true);
    QString name = QHostInfo::localHostName();

    if(!name.isEmpty()){
        host->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if(!domain.isEmpty())
            host->addItem(name + QChar('.') + domain);
    }

    QList<QHostAddress> ipAddresses = QNetworkInterface::allAddresses();

    for(int i = 0; i<ipAddresses.size(); i++)
        if(!ipAddresses.at(i).isLoopback())
            host->addItem(ipAddresses.at(i).toString());

    port->setValidator(new QIntValidator(1, 66500, this));

    auto hostLabel = new QLabel(tr("Host"));
    hostLabel->setBuddy(host);

    auto portLabel = new QLabel(tr("Port"));
    portLabel->setBuddy(port);

    status = new QLabel(tr("Get your quotes!"));
    status->setWordWrap(true);

    getQuote->setEnabled(false);
    getQuote->setDefault(true);

    auto quitButton = new QPushButton(tr("exit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(getQuote, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    read.setDevice(socket);

    connect(host, SIGNAL(editTextChanged(QString)), this, SLOT(enableGetQuote()));
    connect(port, SIGNAL(textChanged(QString)), this, SLOT(enableGetQuote()));
    connect(getQuote, SIGNAL(clicked(bool)), this, SLOT(requestNewQuote()));
    connect(quitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readQuote()));

    QGridLayout * mainLayout = new QGridLayout(this);
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(host, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(port, 1, 1);
    mainLayout->addWidget(status, 2, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    port->setFocus();

    QNetworkConfigurationManager manager;
    QNetworkConfiguration config = manager.defaultConfiguration();
    netSession = new QNetworkSession(config, this);

    connect(netSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

    getQuote->setEnabled(false);
    status->setText(tr("Opening network session"));
    netSession->open();
}

void Client::enableGetQuote(){
    getQuote->setEnabled( (netSession->isOpen() || !netSession)
                         && !port->text().isEmpty()
                         && !host->currentText().isEmpty());
}

void Client::requestNewQuote(){
    getQuote->setEnabled(false);
    socket->abort();
    socket->connectToHost(host->currentText(), port->text().toInt());
}

void Client::readQuote(){
    read.startTransaction();

    QString nextQuote;
    read >> nextQuote;

    if(currentQuote == nextQuote)
        QTimer::singleShot(0, this, SLOT(requestNewQuote()));

    currentQuote = nextQuote;
    status->setText(currentQuote);
    getQuote->setEnabled(true);
    status->setStyleSheet("");
}

void Client::sessionOpened(){
    enableGetQuote();
    status->setText(tr("Client Ready"));
    status->setStyleSheet("color: green; font: bold");
}

