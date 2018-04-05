#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QtWidgets>

#ifndef QT_NO_PRINTER
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#endif

#include "hightlight.h"

#define REP(i, n) for(int i=0; i<n; i++)

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
    void onSaveAsAudio();
    void setFont();

    void countText();
    void checkColumnAndLine();

    void searchOn();
    void find(const QString&);
    void searchoff();

    void addNewWidget();

    void searchAndReplaceOn();
    void replaceAll();

    void print();

    void quit();
private:
    Hightlight * hightlighter;

    QString currentFile;
    Ui::Notepad *ui;

    QString countColumnAndLine();
    QString countWordsAndChars();

#ifndef QT_MO_PRINTER
    QPrinter printer;
#endif

    void setupMenus();
    QMenu * fileMenu;
    QAction * newFile;
    QAction * open;
    QAction * save;
    QAction * saveAs;
    QAction * saveAsAudio;
    QAction * printAct;
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
