#include "digitalshow.h"

#include <QDateTime>
#include <QPropertyAnimation>

DigitalShow::DigitalShow(QWidget * parent)
    : QLCDNumber(parent),
      repeater(new QTimer){
    setSegmentStyle(Filled);

    dotFormat = false;

    display(QString("00:00"));
    repeater->setInterval(1000);
    repeater->start();

    connect(repeater, &QTimer::timeout, [this]{
       dotFormat = !dotFormat;
       QString timeFormat = (dotFormat) ? QDateTime::currentDateTime().toString("hh:mm")
                                        : QDateTime::currentDateTime().toString("hh mm");
       display(timeFormat);
    });

   QPropertyAnimation * animation;
   animation = new QPropertyAnimation(this, "size");
   animation->setStartValue(QSize(0, 0));
   animation->setEndValue(QSize(260, 70));
   animation->start();

   setWindowTitle(tr("Digital Clock"));
}
