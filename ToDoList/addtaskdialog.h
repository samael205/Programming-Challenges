#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QtWidgets>
#include <QObject>

class addTaskDialog : public QDialog
{
    Q_OBJECT
public:
    addTaskDialog(QWidget * parenet = 0);
    QLineEdit * taskName;
    QTextEdit * noteName;
private:
    QPushButton * okButton;
    QPushButton * cancelButton;
    QLabel * taskLabel;
    QLabel * noteLabel;

};

#endif // ADDTASKDIALOG_H
