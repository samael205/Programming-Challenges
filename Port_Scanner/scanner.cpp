#include "scanner.h"
#include "scanstatusdelegate.h"

#include <QTextStream>
#include <QIcon>
#include <QDir>
#include <QPropertyAnimation>
#include <algorithm>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>

#include <QTableView>
#include <QtWidgets>

Scanner::Scanner(QWidget * p)
    : QWidget(p){
    QIcon icon("./icons/scan.png");
    QGridLayout * background;
    background = new QGridLayout;

    data = new QLineEdit[2];
    enter = new QPushButton;
    description = new QLabel;
    description->setText("");
    enter->setIcon(icon);
    enter->setText("Scan");
    enter->setStyleSheet("background-color: #68ADC4;");
    enter->setWindowTitle("Scan ports!");

    QRegExpValidator *v = new QRegExpValidator;
    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.)"
                    "{3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
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
    background->addWidget(description, 0, 1, 0, Qt::AlignVertical_Mask);
    background->addWidget(enter, 0, 2, 2, Qt::AlignRight);
    background->addWidget(data+1, 1, 0);

    scanResults = new ScanInfoModel;

    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(scanResults);
    proxy->setFilterKeyColumn(0);

    QTableView * showResults;
    showResults = new QTableView;
    showResults->setModel(proxy);
    showResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    showResults->setSelectionMode(QAbstractItemView::SingleSelection);
    showResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
    showResults->setSortingEnabled(true);
    showResults->resizeColumnsToContents();
    showResults->horizontalHeader()->setStretchLastSection(true);
    showResults->setColumnWidth(1, 200);

    scanStatusDelegate * delegateModel;
    delegateModel = new scanStatusDelegate;

    showResults->setItemDelegate(delegateModel);

    background->addWidget(showResults, 3, 0, 1, Qt::AlignHorizontal_Mask);

    QProgressBar * progressbar = new QProgressBar(this);
    progressbar->resize(300, 80);
    progressbar->setWindowTitle("Scanning");

    background->addWidget(progressbar, 4, 0, 1, Qt::AlignHorizontal_Mask);
    setLayout(background);

    connect(this, SIGNAL(SignalProgress(int)), progressbar, SLOT(setValue(int)));
    connect(this, SIGNAL(setMax(int)), progressbar, SLOT(setMaximum(int)));
    connect(this, SIGNAL(stop()), progressbar, SLOT(reset()));
    connect(enter, SIGNAL(clicked(bool)), this, SLOT(Scan()));

}

Scanner::~Scanner(){
    delete [] data;
    delete  description;
    delete enter;
}

void Scanner::HostDown(){
    QMessageBox * noHost = new QMessageBox;
    noHost->setWindowTitle("ping error");
    noHost->setText("Destination Host Unreachable");
    noHost->setStandardButtons(QMessageBox::Cancel);
    noHost->setAutoFillBackground(true);
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

QString Scanner::portInfo(int port){
    QFile file(QCoreApplication::applicationDirPath() + "/ports.txt");
    file.open(QIODevice::ReadOnly);
    file.seek((qint64)port);
    QStringList splitContent;
    QTextStream readContent(&file);
    QString line;
    while(!readContent.atEnd()){
        line = readContent.readLine();
        splitContent = line.split(";");
        if( port == splitContent[0].toInt() && splitContent.size() > 1){
            line = splitContent[1];
            break;
        }
        else if(port < splitContent[0].toInt())
            return "  ";
       splitContent.clear();
    }
    file.close();
    return line;
}

void Scanner::RemovePreviousScanInfo(){
    int rows = scanResults->rowCount(QModelIndex());
    scanResults->removeRows(0, rows, QModelIndex());
}

void Scanner::Scan(){
       RemovePreviousScanInfo();
       QString host = data[0].text();

    if(isHostUp(host) && host.length() != 0){
        QString ports = data[1].text();
        QString msg;
        QTextStream show{&msg};

        int i = 0;
        auto portsToCheck = PortList(ports.toStdString());
        emit start();
        emit setMax(portsToCheck.size());

     for(auto & port : portsToCheck){
        if(isOpen(host.toStdString(), port))
            show <<"OPEN";
        else
            show  <<"CLOSE";

         scanResults->insertRows(0, 1, QModelIndex());
         QModelIndex index = scanResults->index(0, 0, QModelIndex());
         scanResults->setData(index, port, Qt::EditRole);
         index = scanResults->index(0, 1, QModelIndex());
         scanResults->setData(index, msg, Qt::EditRole);
         index = scanResults->index(0, 2, QModelIndex());
         scanResults->setData(index, portInfo(port), Qt::EditRole);

         msg.clear();

         emit SignalProgress(++i);
        }
        emit stop();
    }
    else if(host.length() == 0)
        HostEmpty();
    else
        HostDown();
}

void Scanner::saveResultsToCSV(){
    QString filePath = QFileDialog::getSaveFileName(0, "Save File", QDir::currentPath(),
                                                    "(*.csv)");
    QString result;
    QTextStream getContent { &result };

    for(int i = scanResults->rowCount(QModelIndex()); i >= 0; i--){
        QVariant currentData;

        QModelIndex index = scanResults->index(i, 0, QModelIndex());
        currentData = scanResults->data(index, Qt::DisplayRole);
        getContent<<currentData.toString()<<",";
        index = scanResults->index(i, 1, QModelIndex());
        currentData = scanResults->data(index, Qt::DisplayRole);
        getContent<<currentData.toString()<<",";
        index = scanResults->index(i, 2, QModelIndex());
        currentData = scanResults->data(index, Qt::DisplayRole);
        getContent<<currentData.toString()<<"\n";
    }

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QTextStream saveToFile(&file);
    result.trimmed();
    QRegularExpression exp("\"*");
    result.remove(exp);
    saveToFile << result.toUtf8();
    file.flush();
    file.close();
}

vis Scanner::Split(const std::string &s, char delimiter){
    vis tokens;
    std::stringstream sstream(s);
    std::string bufor;
    while(std::getline(sstream, bufor, delimiter))
            tokens.push_back(bufor);
    return tokens;
}

int Scanner::StringToInt(const std::string &s){
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
            case 0:Port_List.push_back(StringToInt(token));break;
            case 1:Port_List.push_back(StringToInt(list_range[0])); break;
        case 2:{
              int min = StringToInt(list_range[0]);
              int max = StringToInt(list_range[1]);
              for(int port : range(min, max))
                  Port_List.push_back(port);
            }break;
        default:
            break;
        }
    }
    return Port_List;
}



