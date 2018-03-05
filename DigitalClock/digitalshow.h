#ifndef DIGITALSHOW_H
#define DIGITALSHOW_H

#include <QLCDNumber>
#include <QTimer>

class DigitalShow : public QLCDNumber{
    Q_OBJECT
public:
    DigitalShow(QWidget * parent = nullptr);
private:
    QTimer * repeater = nullptr;
    bool dotFormat;
};

#endif // DIGITALSHOW_H
