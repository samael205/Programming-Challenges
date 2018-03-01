#include "chatdialog.h"
#include "ui_chatdialog.h"

#include <QTextTable>
#include <QScrollBar>
#include <QListWidgetItem>

ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog){

    ui->setupUi(this);

    ui->textEdit->setReadOnly(true);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit->setFocusPolicy(Qt::StrongFocus);

    tableFormat.setBorder(0);

    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(clickedToSendMessage()));

    connect(&client, SIGNAL(newUser(QString)), this, SLOT(newUser(QString)));
    connect(&client, SIGNAL(newMessage(QString,QString)), this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(userLeft(QString)), this, SLOT(userLeft(QString)));
}

ChatDialog::~ChatDialog(){
    delete ui;
}

void ChatDialog::appendMessage(const QString& from, const QString& content){
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);

    QTextTable * table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText("<" + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(content);

    QScrollBar * bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::newUser(const QString & user){
    if(user.isEmpty())
        return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("%1 has join").arg(user));
    ui->textEdit->setTextColor(color);
    ui->listWidget->addItem(user);
}

void ChatDialog::userLeft(const QString & user){
    if(user.isEmpty())
        return;

    QList<QListWidgetItem*> users = ui->listWidget->findItems(user, Qt::MatchExactly);
    if(users.isEmpty())
        return;

    delete users.at(0);

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("%1 has left").arg(user));
    ui->textEdit->setTextColor(color);
}

void ChatDialog::clickedToSendMessage(){
    QString messageToSend = ui->lineEdit->text();

    client.sendMessage(messageToSend);

    ui->lineEdit->clear();
}
