#ifndef DIALOG_H
#define DIALOG_H

#include "threadserver.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class Dialog : public QWidget
{
    Q_OBJECT
public:
    Dialog(QWidget * parent = 0);
private:
    QLabel * status;
    QPushButton * quit;
    ThreadServer server;
};

#endif // DIALOG_H
