#ifndef ADDTASKDIALOG_H
#define ADDTASKDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QtWidgets>
#include <QObject>

class addTaskDialog : public QDialog{
    Q_OBJECT
public:
    addTaskDialog(QWidget * parent = nullptr);

    QLineEdit * taskName = nullptr;
    QTextEdit * noteName = nullptr;
};

#endif // ADDTASKDIALOG_H
