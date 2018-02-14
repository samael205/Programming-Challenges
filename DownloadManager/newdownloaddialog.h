#ifndef NEWDOWNLOADDIALOG_H
#define NEWDOWNLOADDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class newDownloadDialog : public QDialog
{
    Q_OBJECT
public:
    newDownloadDialog();
    QLineEdit * linkToData;
private:
    void setup();
    QLabel * linkLabel;
    QPushButton * okButton;
    QPushButton * cancelButton;
};

#endif // NEWDOWNLOADDIALOG_H
