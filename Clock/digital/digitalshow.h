#ifndef DIGITALSHOW_H
#define DIGITALSHOW_H

#include <QLCDNumber>
#include <QTimer>

class DigitalShow : public QWidget {
    Q_OBJECT
public:
    DigitalShow(QWidget * parent = nullptr);
private:
    QTimer * repeater = nullptr;
    bool dotFormat;
    QLCDNumber * digitalClock = nullptr;
};

#endif // DIGITALSHOW_H
