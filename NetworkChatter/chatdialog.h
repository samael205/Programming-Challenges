#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QTextTableFormat>

#include "client.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog{
    Q_OBJECT
public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
public slots:
    void appendMessage(const QString&, const QString&);
    void newUser(const QString&);
    void userLeft(const QString&);
    void clickedToSendMessage();
private:
    Ui::ChatDialog *ui;

    QTextTableFormat tableFormat;
    Client client;
};

#endif // CHATDIALOG_H
