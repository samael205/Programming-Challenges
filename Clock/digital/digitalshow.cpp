#include "digitalshow.h"

#include <QDateTime>
#include <QPropertyAnimation>
#include <QGridLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QWindow>

DigitalShow::DigitalShow(QWidget * parent)
    : QWidget(parent),
      repeater(new QTimer),
      digitalClock(new QLCDNumber){
    digitalClock->setSegmentStyle(QLCDNumber::Filled);

    dotFormat = false;

    digitalClock->display(QString("00:00"));
    repeater->setInterval(1000);
    repeater->start();

    connect(repeater, &QTimer::timeout, [this]{
       dotFormat = !dotFormat;
       QString timeFormat = (dotFormat) ? QDateTime::currentDateTime().toString("hh:mm")
                                        : QDateTime::currentDateTime().toString("hh mm");
       digitalClock->display(timeFormat);
    });

   QString currentData = QDateTime::currentDateTime().toString("dd/MM/yyyy");
   auto currentDataLabel = new QLabel(currentData);
   currentDataLabel->setStyleSheet("font: 40px; font-weight: 200;");

   auto dateLabelShadow = new QGraphicsDropShadowEffect;
   dateLabelShadow->setOffset(QPoint(3, 4));
   dateLabelShadow->setBlurRadius(2);
   currentDataLabel->setGraphicsEffect(dateLabelShadow);

   QPropertyAnimation * animation;
   animation = new QPropertyAnimation(this, "size");
   animation->setStartValue(QSize(0, 0));
   animation->setEndValue(QSize(360, 170));
   animation->start();

   auto mainLayout = new QGridLayout;
   mainLayout->addWidget(digitalClock);
   mainLayout->addWidget(currentDataLabel, 1, 0, Qt::AlignCenter);
   setLayout(mainLayout);

   setWindowTitle(tr("Digital Clock"));
}
