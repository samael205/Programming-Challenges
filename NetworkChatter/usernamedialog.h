#ifndef USERNAMEDIALOG_H
#define USERNAMEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class usernameDialog : public QDialog{
    Q_OBJECT
public:
    usernameDialog(QWidget * parent = nullptr);
    QLineEdit * username = nullptr;
private:
    QPushButton * okButton = nullptr;
    QPushButton * cancelButton = nullptr;
};

#endif // USERNAMEDIALOG_H
