#include "scanner.h"

#include <QTextStream>
#include <QIcon>
#include <QDir>
#include <QPropertyAnimation>
#include <algorithm>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>

Scanner::Scanner(QWidget * p) : QWidget(p)
{
    QIcon icon(QCoreApplication::applicationDirPath() + "/scan.png");
    QGridLayout * background;
    background = new QGridLayout;

    open = new QCheckBox;
    data = new QLineEdit[2];
    output = new QTextEdit;
    enter = new QPushButton;
    description = new QLabel[2];
    description[0].setText("Scan");
    description[1].setText("Open");
    enter->setIcon(icon);
    enter->setStyleSheet("background-color: #68ADC4;");
    enter->setWindowTitle("Scan ports!");
    filter = 1;

    QRegExpValidator *v = new QRegExpValidator;
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    v->setRegExp(rx);
    QRegExpValidator *vx = new QRegExpValidator;
    QRegExp rc("[0-9,-]+");
    vx->setRegExp(rc);

    data[0].setWindowTitle("Host addres");
    data[0].setValidator(v);
    data[0].setPlaceholderText("Host");
    data[1].setWindowTitle("Set Ports to scan");
    data[1].setPlaceholderText("Ports");
    data[1].setValidator(vx);
    background->addWidget(data, 0, 0);
    background->addWidget(description, 0, 1);
    background->addWidget(enter, 0, 2);
    background->addWidget(data+1, 1, 0);
    background->addWidget(description+1, 1, 1);
    background->addWidget(open, 1, 2);
    background->addWidget(output, 3, 0);
    setLayout(background);

    QPropertyAnimation * animation = new QPropertyAnimation(this, "geometry");
    QRect start(500, 200, 0, 0);
    setGeometry(start);
    QRect begin = geometry();
    QRect end(begin.x(), begin.y(), 650, 500);
    animation->setStartValue(begin);
    animation->setEndValue(end);
    animation->start();
    QDialog * progress = new QDialog;

    progress->resize(300, 80);
    progress->setToolTip("Wait until ports is scanning");
    progress->setWindowTitle("Scanning");
    QProgressBar * progressbar = new QProgressBar(progress);
    progressbar->resize(300, 80);
    progressbar->setWindowTitle("Scanning");

    connect(this, SIGNAL(SignalProgress(int)), progressbar, SLOT(setValue(int)));
    connect(this, SIGNAL(setMax(int)), progressbar, SLOT(setMaximum(int)));
    connect(this, SIGNAL(stop()), progress, SLOT(hide()));
    connect(this, SIGNAL(start()), progress, SLOT(open()));
    connect(enter, SIGNAL(clicked(bool)), this, SLOT(Scan()));
    connect(open, SIGNAL(clicked(bool)), this, SLOT(ChangeFilter()));
}

Scanner::~Scanner(){
    delete [] data;
    delete [] description;
    delete output;
    delete enter;\
}

void Scanner::HostDown(){
    QMessageBox * noHost = new QMessageBox;
    noHost->setWindowTitle("ping error");
    noHost->setText("Destination Host Unreachable");
    noHost->setStandardButtons(QMessageBox::Cancel);
    noHost->exec();
    delete noHost;
}
void Scanner::HostEmpty(){
    QMessageBox * noHost = new QMessageBox;
    noHost->setWindowTitle("host name error");
    noHost->setText("Write host correct");
    noHost->setStandardButtons(QMessageBox::Cancel);
    noHost->exec();
    delete noHost;
}

bool Scanner::isHostUp(const QString &host){
    QStringList parameters;
#if defined(WIN32)
    parameters << "-n"<<"1";
#else
    parameters << "-c" <<"1";
#endif
    parameters<<host;
    int exitCode = QProcess::execute("ping", parameters);
    if(exitCode == 0)
        return true;
    else
        return false;
}

std::string Scanner::port_info(int port){
    std::fstream file;
    file.open(QCoreApplication::applicationDirPath().toStdString() + "/ports.txt");
   if(!file.is_open())
        exit(EXIT_FAILURE);
    file.seekg(port);
    std::string line;
    while(std::getline(file, line))
        if(line.substr(0, line.find_first_of("\t")) == std::to_string(port)){
            line =  line.substr(line.find_first_of("\t"), line.length());
            break;
        }
        else if(std::atoi(line.substr(0, line.find_first_of("\t")).c_str()) > port){
            line = "";
            break;
        }
    file.close();
    return line;
}

void Scanner::Scan(){
      std::string host = data[0].text().toStdString();
      output->clear();
    if(isHostUp(QString::fromStdString(host)) && host.length() != 0){
        std::string ports = data[1].text().toStdString();
        QString msg;
        QTextStream show{&msg};
        int i = 0;
       auto Ports_To_Check = PortList(ports);
       emit setMax(Ports_To_Check.size());
       emit start();
     for(auto & port : Ports_To_Check){
         switch(filter){
         case 0:
             if(isOpen(host, port)){
                 show <<QString::number(port) <<"\tOPEN ";
                 show<<QString::fromStdString(port_info(port))<<"\n";
             }
              break;
         case 1:
                if(isOpen(host, port))
                 show << QString::number(port) <<"\tOPEN ";
                else
                  show << QString::number(port) <<"\tCLOSE ";
               show<<QString::fromStdString(port_info(port))<<"\n";
               break;
         }
         emit SignalProgress(++i);
     }
      emit stop();
      emit setMax(0);
      emit SignalProgress(0);
     output->insertPlainText(msg);
    }
    else if(host.length() == 0)
        HostEmpty();
    else
        HostDown();
}

vis Scanner::Split(const std::string &s, char delimiter){
    vis tokens;
    std::stringstream sstream(s);
    std::string bufor;
    while(std::getline(sstream, bufor, delimiter))
            tokens.push_back(bufor);
    return tokens;
}

int Scanner::String_To_Int(const std::string &s){
    std::stringstream sstream(s);
    int IntFromString;
    sstream >> IntFromString;
    return IntFromString;
}

template <class T>
std::vector<T> Scanner::range(T max, T min){
    if(min > max)
        Swap(min, max);
    if(min == max)
        return std::vector<T>(1, min);
    std::vector<T> setRange;
    for(; min <= max; min++)
        setRange.push_back(min);
    return setRange;
}

template <class T>
void Scanner::Swap(T & changeToSecond, T & changeToFirst){
    T temp;
    temp = changeToSecond;
    changeToSecond = changeToFirst;
    changeToFirst = temp;
}

vi Scanner::PortList(const std::string & ports){
    vi Port_List;
    for(const std::string & token : Split(ports, ',')){
        vis list_range = Split(token, '-');
        switch(list_range.size()){
            case 0:Port_List.push_back(String_To_Int(token));break;
            case 1:Port_List.push_back(String_To_Int(list_range[0])); break;
        case 2:{
              int min = String_To_Int(list_range[0]);
              int max = String_To_Int(list_range[1]);
              for(int port : range(min, max))
                  Port_List.push_back(port);
            }break;
        default:
            break;
        }
    }
    return Port_List;
}
