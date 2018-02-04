#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QtWidgets>

#include "hightlight.h"

namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();
public slots:
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void setFont();

    void countAll();

    void searchOn();
    void find(const QString&);
    void searchoff();

    void addNewWidget();

    void searchAndReplaceOn();
    void replaceAll();

    void quit();
private:
    Hightlight * hightlighter;

    QString currentFile;
    Ui::Notepad *ui;

    QString countColumnAndLine();
    QString countWordsAndChars();

    void setupMenus();
    QMenu * fileMenu;
    QAction * newFile;
    QAction * open;
    QAction * save;
    QAction * saveAs;
    QAction * exit;

    QMenu * edit;
    QAction * search;
    QAction * replace;

    QMenu *fontMenu;
    QAction * fontSelect;

    QMenu * viewMenu;
    QAction * newView;
};

#endif // NOTEPAD_H
