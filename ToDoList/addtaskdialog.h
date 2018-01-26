#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class addTaskDialog : public QDialog
{
    Q_OBJECT
public:
    addTaskDialog(QWidget * parenet = 0);
    QLineEdit * taskName;
private:
    QPushButton * okButton;
    QPushButton * cancelButton;
    QLabel * taskLabel;

};

#endif // ADDTASKDIALOG_H
