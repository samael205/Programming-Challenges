#include "notepad.h"
#include "ui_notepad.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QMenuBar>
#include <QGridLayout>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    ui->setupUi(this);

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(countWords()));

    setupMenus();
}

Notepad::~Notepad()
{
    delete ui;
}

void Notepad::setupMenus(){
    fileMenu = menuBar()->addMenu("&File");

    newFile = new QAction(tr("&New"), this);
    fileMenu->addAction(newFile);
    connect(newFile, SIGNAL(triggered(bool)), this, SLOT(on_ActionNew_Trigerred()));

    openFile = new QAction(("&Open"), this);
    fileMenu->addAction(openFile);
    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(on_Action_Trigerred()));

    saveFile = new QAction(tr("&Save"), this);
    fileMenu->addAction(saveFile);
    connect(saveFile, SIGNAL(triggered(bool)), this, SLOT(on_ActionSave_Trigerred()));

    saveAsFile = new QAction(tr("&Save as"), this);
    fileMenu->addAction(saveAsFile);
    connect(saveAsFile, SIGNAL(triggered(bool)), this, SLOT(on_ActionSave_as_Trigerred()));

    fontMenu = menuBar()->addMenu("Font");

    setFont = new QAction(tr("&Set Font"), this);
    fontMenu->addAction(setFont);
    connect(setFont, SIGNAL(triggered(bool)), this, SLOT(on_ActionFont_Trigerred()));

    view = menuBar()->addMenu(tr("&View"));
    newWindow = new QAction(tr("&New"), this);
    view->addAction(newWindow);
    connect(newWindow, SIGNAL(triggered(bool)), this, SLOT(window()));
}

void Notepad::window(){
    Notepad * quickWindow;
    quickWindow = new Notepad;
    quickWindow->show();
}

void Notepad::on_ActionNew_Trigerred(){
    ui->textEdit->setText("");
    currentFile = "";
}

void Notepad::on_Action_Trigerred(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open File");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, tr("Error!"), tr("File not opened"));
        return;
    }
    QTextStream getContent(&file);
    QString text = getContent.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::on_ActionSave_Trigerred(){
    QFile file(currentFile);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("..."), tr("File not opened"));
        return;
    }
    QTextStream saveTo(&file);
    QString text = ui->textEdit->toPlainText();
    saveTo << text;
    file.flush();
    file.close();
}

void Notepad::on_ActionSave_as_Trigerred(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"));
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, tr("..."), "File not opened");
        return;
    }
    QTextStream writeContent(&file);
    QString text = ui->textEdit->toPlainText();
    writeContent << text;
    file.flush();
    file.close();
}

void Notepad::on_ActionFont_Trigerred(){
    bool select;
    QFont font = QFontDialog::getFont(&select, this);
    if(select)
       ui->textEdit->setFont(font);
}

unsigned int Notepad::calculate(QString & sentence){
    std::stringstream sstream(sentence.toStdString());
    return std::distance(std::istream_iterator<std::string>(sstream), std::istream_iterator<std::string>());
}

unsigned int Notepad::countChars(const std::string & word){
    int count = 0;
    for(auto it = word.begin(); it != word.end(); it++)
        if(std::isalpha(*it) || std::ispunct(*it))
            count++;
    return count;
}

void Notepad::countWords(){
    QString text = ui->textEdit->toPlainText();
    auto count = calculate(text);
    auto chars = countChars(text.toStdString());
    ui->label->setText("Words: " + QString::number(count) + " Chars: " + QString::number(chars));
}


