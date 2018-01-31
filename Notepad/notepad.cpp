#include "notepad.h"
#include "ui_notepad.h"
#include <QMessageBox>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad){

    ui->setupUi(this);

    searchoff();

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(countAll()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(searchoff()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(find(QString)));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(replaceAll()));

    QString replaceIconPath = QDir::currentPath() + "/icons/exit.png";
    QString searchIconPath = QDir::currentPath() + "/icons/replace.png";

    QIcon replaceIcon(replaceIconPath);
    QIcon searchIcon(searchIconPath);

    ui->pushButton->setIcon(replaceIcon);
    ui->pushButton_2->setIcon(searchIcon);

    setupMenus();
}

Notepad::~Notepad(){
    delete ui;
}

void Notepad::setupMenus(){
    fileMenu = menuBar()->addMenu("File");

    newFile = new QAction("New", this);
    fileMenu->addAction(newFile);
    connect(newFile, SIGNAL(triggered(bool)), this, SLOT(onNew()));

    open = new QAction("Open", this);
    fileMenu->addAction(open);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(onOpen()));

    save = new QAction("Save", this);
    fileMenu->addAction(save);
    connect(save, SIGNAL(triggered(bool)), this, SLOT(onSave()));

    saveAs = new QAction("Save As", this);
    fileMenu->addAction(saveAs);
    connect(saveAs, SIGNAL(triggered(bool)), this, SLOT(onSaveAs()));

    edit = menuBar()->addMenu("Edit");

    search = new QAction("Find", this);
    edit->addAction(search);
    connect(search, SIGNAL(triggered(bool)), this, SLOT(searchOn()));

    replace = new QAction("Replace", this);
    edit->addAction(replace);
    connect(replace, SIGNAL(triggered(bool)), this, SLOT(searchAndReplaceOn()));

    fontMenu = menuBar()->addMenu("Font");

    fontSelect = new QAction("Set new font", this);
    fontMenu->addAction(fontSelect);
    connect(fontSelect, SIGNAL(triggered(bool)), this, SLOT(setFont()));

    viewMenu = menuBar()->addMenu("View");
    newView = new QAction("New", this);
    viewMenu->addAction(newView);
    connect(newView, SIGNAL(triggered(bool)), this, SLOT(addNewWidget()));
}

void Notepad::addNewWidget(){
    Notepad * addNotepad;
    addNotepad = new Notepad;
    addNotepad->show();
}

void Notepad::onNew(){
    currentFile = "";
    ui->textEdit->setText("");
}

void Notepad::onOpen(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open File");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::information(this, "File open error", tr("Could not open %1 file").arg(fileName));
        return;
    }
    QTextStream getFromFile(&file);
    QString text = getFromFile.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void Notepad::onSave(){

    QFile file(currentFile);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::information(this, "Write Error!", tr("No file opened!"));
        return;
    }
    QTextStream getFromNotepad(&file);
    QString text = ui->textEdit->toPlainText();
    getFromNotepad << text;
    file.flush();
    file.close();
}

void Notepad::onSaveAs(){
    QString filePath = QFileDialog::getSaveFileName(this, "Save file");
    QFile file(filePath);
    currentFile = filePath;
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        QMessageBox::information(this, "Write error", tr("File not opened!"));
        return;
    }
    QTextStream getFromNotepad(&file);
    QString text = ui->textEdit->toPlainText();
    getFromNotepad << text;
    file.flush();
    file.close();
}

void Notepad::setFont(){
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if(fontSelected)
        ui->textEdit->setFont(font);
}

void Notepad::countAll(){
    QString format = countColumnAndLine() + countWordsAndChars();
    ui->counter->setText(format);
}

QString Notepad::countColumnAndLine(){
    QTextCursor cursor = ui->textEdit->textCursor();
    int column = cursor.blockNumber()+1;
    int line = cursor.columnNumber()+1;
    QString format = "Column: " + QString::number(column) + ", Line: " + QString::number(line);
    return format;
}

QString Notepad::countWordsAndChars(){
    std::string text = ui->textEdit->toPlainText().toStdString();
    std::stringstream sstream(text);
    int words =  std::distance(std::istream_iterator<std::string>(sstream), std::istream_iterator<std::string>());

    int chars = 0;
    for(auto it = text.begin(); it != text.end(); it++)
        if(!std::isspace(*it))
            chars++;
    return "\tWords: " + QString::number(words) + ", Chars: " + QString::number(chars);
}

void Notepad::searchoff(){
    ui->searcher->hide();
    ui->counter->show();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Notepad::searchOn(){
    ui->searcher->show();
    ui->lineEdit_2->hide();
    ui->pushButton_2->hide();
    ui->label_2->hide();

    ui->counter->hide();
    ui->lineEdit->setFocus();
}

void Notepad::find(const QString & find){
   hightlighter = new Hightlight(ui->textEdit->document(), find);
}

void Notepad::searchAndReplaceOn(){
    ui->counter->hide();

    ui->searcher->show();
    ui->lineEdit_2->show();
    ui->pushButton_2->show();
    ui->label_2->show();
}

void Notepad::replaceAll(){
    QString text = ui->textEdit->toPlainText();

    QString toReplace = ui->lineEdit->text();
    QString replaceWith = ui->lineEdit_2->text();

    text.replace(toReplace, replaceWith);

    ui->textEdit->setText(text);
}
