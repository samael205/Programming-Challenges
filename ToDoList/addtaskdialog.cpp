#include "addtaskdialog.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>

addTaskDialog::addTaskDialog(QWidget * parent)
    : QDialog(parent){ 
    taskName = new QLineEdit;
    taskName->setPlaceholderText(("Title"));
    QLabel *taskLabel = new QLabel("Task");

    QGridLayout * descriptionLayout = new QGridLayout;
    descriptionLayout->addWidget(taskLabel, 0, 0);
    descriptionLayout->addWidget(taskName, 0, 1);

    QLabel * noteLabel = new QLabel("Note");
    noteName = new QTextEdit;
    noteName->setPlaceholderText(tr("Description"));
    noteName->setFixedHeight(100);

    descriptionLayout->addWidget(noteLabel, 1, 0);
    descriptionLayout->addWidget(noteName, 1, 1);

    QPushButton * okButton = new QPushButton("ok");
    QPushButton * cancelButton = new QPushButton("cancel");

    QHBoxLayout * buttons = new QHBoxLayout;
    buttons->addWidget(okButton);
    buttons->addWidget(cancelButton);

    descriptionLayout->addLayout(buttons, 2, 1, Qt::AlignRight);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(descriptionLayout);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    setWindowTitle("Add new task");
    setLayout(mainLayout);
}
