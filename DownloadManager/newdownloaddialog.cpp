#include "newdownloaddialog.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

newDownloadDialog::newDownloadDialog(){
    setup();

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

void newDownloadDialog::setup(){
    QGridLayout * dataLayout;
    dataLayout = new QGridLayout;

    linkToData = new QLineEdit;
    linkLabel = new QLabel(tr("Url "));
    dataLayout->addWidget(linkLabel, 0, 0);
    dataLayout->addWidget(linkToData, 0, 1);

    QHBoxLayout * buttonsLayout;
    buttonsLayout = new QHBoxLayout;

    okButton = new QPushButton("ok");
    cancelButton = new QPushButton("cancel");
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    dataLayout->addLayout(buttonsLayout, 1, 1, Qt::AlignRight);

    QVBoxLayout * mainLayout;
    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dataLayout);

    setWindowTitle(tr("New Download"));
    resize(270, 100);
    setLayout(mainLayout);
}
