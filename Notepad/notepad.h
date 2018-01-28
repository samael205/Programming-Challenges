#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

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
    void on_ActionNew_Trigerred();
    void on_Action_Trigerred();
    void on_ActionSave_Trigerred();
    void on_ActionSave_as_Trigerred();
    void on_ActionFont_Trigerred();

    void countWords();

    void window();
protected:
    unsigned int calculate(QString&);
    unsigned int countChars(const std::string&);
private:
    Ui::Notepad *ui;
    QString currentFile;

    void setupMenus();
    QMenu * fileMenu;
    QAction * saveFile;
    QAction * saveAsFile;
    QAction * openFile;
    QAction * newFile;

    QMenu * fontMenu;
    QAction * setFont;

    QMenu *view;
    QAction * newWindow;
};

#endif // NOTEPAD_H
