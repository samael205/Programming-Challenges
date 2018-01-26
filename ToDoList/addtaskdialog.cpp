#include "addtaskdialog.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>

addTaskDialog::addTaskDialog(QWidget * parent)
    : QDialog(parent){
    taskName = new QLineEdit;
    taskLabel = new QLabel("Task");
    QGridLayout * descriptionLayout = new QGridLayout;
    descriptionLayout->addWidget(taskLabel, 0, 0);
    descriptionLayout->addWidget(taskName, 0, 1);

    okButton = new QPushButton("ok");
    cancelButton = new QPushButton("cancel");

    QHBoxLayout * buttons = new QHBoxLayout;
    buttons->addWidget(okButton);
    buttons->addWidget(cancelButton);

    descriptionLayout->addLayout(buttons, 1, 1, Qt::AlignRight);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(descriptionLayout);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    setWindowTitle("Add new task");
    setLayout(mainLayout);
}