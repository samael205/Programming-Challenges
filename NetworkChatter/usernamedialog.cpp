#include "usernamedialog.h"

#include <QHBoxLayout>
#include <QGridLayout>

usernameDialog::usernameDialog(QWidget * parent)
    : QDialog(parent){

    auto usernameLabel = new QLabel(tr("Set your nickname"));
    username = new QLineEdit;

    okButton = new QPushButton(tr("ok"));
    cancelButton = new QPushButton(tr("cancel"));

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    auto mainLayout = new QGridLayout;

    mainLayout->addWidget(usernameLabel, 0, 0, Qt::AlignCenter);
    mainLayout->addWidget(username, 1, 0);
    mainLayout->addLayout(buttonsLayout, 2, 0, Qt::AlignRight);

    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    setWindowTitle(tr("Nickname"));
    resize(220, 60);
}
