#include "notepad.h"
#include "ui_notepad.h"
#include <Python.h>
#include <QMessageBox>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <QDebug>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad){

    ui->setupUi(this);

    searchoff();

    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(countText()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(searchoff()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(find(QString)));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(replaceAll()));
    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(checkColumnAndLine()));

    QString replaceIconPath = QDir::currentPath() + "/icons/exit.png";
    QString searchIconPath = QDir::currentPath() + "/icons/replace.png";

    QIcon replaceIcon(replaceIconPath);
    QIcon searchIcon(searchIconPath);

    ui->pushButton->setIcon(replaceIcon);
    ui->pushButton_2->setIcon(searchIcon);

    setupMenus();

    QPropertyAnimation * animation;
    animation = new QPropertyAnimation(this, "geometry");
    QRect screenSize = QApplication::desktop()->screenGeometry();
    int x = screenSize.width();
    int y = screenSize.height();
    animation->setEndValue(QRect(x/3, y/3, 700, 500));
    animation->start();
}

Notepad::~Notepad(){
    delete ui;
}

void Notepad::setupMenus(){
    fileMenu = menuBar()->addMenu("File");

    newFile = new QAction("New", this);
    fileMenu->addAction(newFile);
    newFile->setShortcut(QKeySequence::New);
    connect(newFile, SIGNAL(triggered(bool)), this, SLOT(onNew()));

    fileMenu->addSeparator();

    open = new QAction("Open", this);
    fileMenu->addAction(open);
    open->setShortcut(QKeySequence::Open);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(onOpen()));

    save = new QAction("Save", this);
    fileMenu->addAction(save);
    save->setShortcut(QKeySequence::Save);
    connect(save, SIGNAL(triggered(bool)), this, SLOT(onSave()));

    saveAs = new QAction("Save As", this);
    fileMenu->addAction(saveAs);
    saveAs->setShortcut(QKeySequence::SaveAs);
    connect(saveAs, SIGNAL(triggered(bool)), this, SLOT(onSaveAs()));

    saveAsAudio = new QAction("Save As Audio", this);
    fileMenu->addAction(saveAsAudio);
    saveAsAudio->setShortcut(QKeySequence(tr("ALT+SHIFT+S")));
    connect(saveAsAudio, SIGNAL(triggered(bool)), this, SLOT(onSaveAsAudio()));

    fileMenu->addSeparator();

    printAct = new QAction("Print", this);
    fileMenu->addAction(printAct);
    printAct->setShortcut(QKeySequence::Print);
    connect(printAct, SIGNAL(triggered(bool)), this, SLOT(print()));

    fileMenu->addSeparator();

    exit = new QAction("Exit", this);
    fileMenu->addAction(exit);
    exit->setShortcut(QKeySequence::Quit);
    connect(exit, SIGNAL(triggered(bool)), this, SLOT(quit()));

    edit = menuBar()->addMenu("Edit");

    search = new QAction("Find", this);
    edit->addAction(search);
    search->setShortcut(QKeySequence::Find);
    connect(search, SIGNAL(triggered(bool)), this, SLOT(searchOn()));

    replace = new QAction("Replace", this);
    edit->addAction(replace);
    replace->setShortcut(QKeySequence::Replace);
    connect(replace, SIGNAL(triggered(bool)), this, SLOT(searchAndReplaceOn()));

    fontMenu = menuBar()->addMenu("Font");

    fontSelect = new QAction("Set new font", this);
    fontMenu->addAction(fontSelect);
    connect(fontSelect, SIGNAL(triggered(bool)), this, SLOT(setFont()));

    viewMenu = menuBar()->addMenu("View");
    newView = new QAction("New Window", this);
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

    statusBar()->showMessage(tr("Opened file %1").arg(QDir::toNativeSeparators(fileName)));
}

void Notepad::onSave(){

    QFile file(currentFile);
    if(!file.open(QIODevice::WriteOnly | QFile::Text)){
        statusBar()->showMessage("No file opened!");
        return;
    }
    QTextStream getFromNotepad(&file);
    QString text = ui->textEdit->toPlainText();
    getFromNotepad << text;
    file.flush();
    file.close();

    statusBar()->showMessage("Saved");
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

    statusBar()->showMessage(tr("Saved as %1").arg(filePath.split("/").back()));
}

void Notepad::onSaveAsAudio(){
    QString filePath = QFileDialog::getSaveFileName(this, "Save Audio File", QDir::homePath(), tr("Audio(*.mp3 *.wav)"));

    if(filePath.isEmpty())
        return;

    Py_Initialize();
    PyObject * module = PyImport_ImportModule("text_to_audio");
    PyObject * dict = PyModule_GetDict(module);
    PyObject * arguments = PyTuple_New(2);
    PyObject * function = PyDict_GetItem(dict, PyString_FromString("convert_to_audio"));
    QString textContent = ui->textEdit->toPlainText();
    PyTuple_SetItem(arguments, 0, PyString_FromString(textContent.toStdString().c_str()));
    PyTuple_SetItem(arguments, 1, PyString_FromString(filePath.toStdString().c_str()));

    if(PyErr_Occurred()){
        PyErr_Print();
        Py_Finalize();
        return;
    }

    PyObject_CallObject(function, arguments);
    Py_Finalize();

    statusBar()->showMessage(tr("Saved as %1").arg(filePath.split("/").back()));
}

void Notepad::setFont(){
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if(fontSelected){
        ui->textEdit->setFont(font);
        statusBar()->showMessage(tr("New font %1")
                .arg(font.family()));
    }
}

void Notepad::print(){
     QPrintDialog dialog(&printer, this);
     if(dialog.exec())
         ui->textEdit->print(&printer);
}

void Notepad::countText(){
    ui->wordCounter->setText(countWordsAndChars());
}

QString Notepad::countColumnAndLine(){
    QTextCursor cursor = ui->textEdit->textCursor();
    int column = cursor.blockNumber()+1;
    int line = cursor.columnNumber()+1;
    QString format = "Column: " + QString::number(column) + ", Line: " + QString::number(line);
    return format;
}

void Notepad::checkColumnAndLine(){
    ui->positionCounter->setText(countColumnAndLine());
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
    ui->wordCounter->show();
    ui->positionCounter->show();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Notepad::searchOn(){
    ui->searcher->show();
    ui->lineEdit_2->hide();
    ui->pushButton_2->hide();
    ui->label_2->hide();

    ui->wordCounter->hide();
    ui->positionCounter->hide();
    ui->lineEdit->setFocus();
}

void Notepad::find(const QString & find){
   hightlighter = new Hightlight(ui->textEdit->document(), find);

   QString cursorPositionOnWord = ui->textEdit->toPlainText();
   QTextCursor setCursorPosition = ui->textEdit->textCursor();
   int wordPosition = cursorPositionOnWord.indexOf(find); 
   std::string wordExistInNotepad = ui->textEdit->toPlainText().toStdString();
   std::string searchedWord = ui->lineEdit->text().toStdString();
    
   if(wordExistInNotepad.find(searchedWord) != std::string::npos){
       ui->lineEdit->setStyleSheet("background-color: green;");
       setCursorPosition.setPosition(wordPosition);
       setCursorPosition.setPosition(wordPosition + find.length(), QTextCursor::KeepAnchor);
       ui->textEdit->setTextCursor(setCursorPosition);
   }
   else
       ui->lineEdit->setStyleSheet("background-color: red;");

   if(find.length() > 0 && find != " "){
       QString countSearchedWord = ui->textEdit->toPlainText();
       ui->label_3->setText(tr("Count: %1").arg(countSearchedWord.count(find)));
   }
   else
       ui->label_3->setText("Count: 0");
}

void Notepad::searchAndReplaceOn(){
    ui->wordCounter->hide();
    ui->positionCounter->hide();

    ui->searcher->show();
    ui->lineEdit_2->show();
    ui->pushButton_2->show();
    ui->label_2->show();

    ui->lineEdit->setFocus();
}

void Notepad::replaceAll(){
    QString text = ui->textEdit->toPlainText();

    QString toReplace = ui->lineEdit->text();
    QString replaceWith = ui->lineEdit_2->text();

    text.replace(toReplace, replaceWith);

    ui->textEdit->setText(text);
}

void Notepad::quit(){
    QGuiApplication::exit();
}
