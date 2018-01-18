#ifndef SCAN_H
#define SCAN_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QRegExp>
#include <QRegExpValidator>
#include <QProgressBar>
#include <QTimer>
#include <QProgressDialog>
#include <QCheckBox>
#include <QLabel>
#include <QFutureWatcher>

#include <string>
#include <fstream>
#include <sstream>
#include <SFML/Network.hpp>

typedef std::vector<std::string> vis;
typedef std::vector<int> vi;

class Scanner : public QWidget
{
    Q_OBJECT
public:
    explicit Scanner(QWidget * p = 0);
    ~Scanner();
    bool isOpen(const std::string &addres, int port){
        return sf::TcpSocket().connect(addres, port) == sf::Socket::Done;
    }
private:
    QLineEdit * data;
    QTextEdit * output;
    QPushButton * enter;
    QCheckBox * open;
    QLabel * description;
    bool filter;
protected:
    std::string port_info(int);
    vi PortList(const std::string &);
    vis Split(const std::string & s, char delimiter = ' ');
    int String_To_Int(const std::string & s);
    bool isHostUp(const QString & host);

    void HostDown();
    void HostEmpty();

    template <class T>
    std::vector<T> range(T max, T min);

    template <class T>
    void Swap(T & a, T & b);

signals:
    void SignalProgress(int);
    void setMax(int);
    void start();
    void stop();
private slots:
    void Scan();
    void ChangeFilter() { filter = !filter;}
};

#endif // SCAN_H
