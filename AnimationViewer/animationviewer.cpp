#include "animationviewer.h"
#include <QPropertyAnimation>

AnimationViewer::AnimationViewer(QWidget *parent)
    : QWidget(parent){
    animated = new QMovie(this);
    animated->setCacheMode(QMovie::CacheAll);

    animatedLabel = new QLabel(tr("No Animation loaded!"));
    animatedLabel->setStyleSheet("font-weight: 800;");
    animatedLabel->setAlignment(Qt::AlignCenter);
    animatedLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    animatedLabel->setBackgroundRole(QPalette::Dark);
    animatedLabel->setAutoFillBackground(true);

    currentAnimationDirectory = "Pictures";

    mainLayout = new QVBoxLayout;

    createControls();
    createButtons();

    mainLayout->addWidget(animatedLabel);
    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    connect(animated, SIGNAL(frameChanged(int)), this, SLOT(updateFrameSlider));
    connect(animated, SIGNAL(stateChanged(QMovie::MovieState)), this, SLOT(updateButtons()));
    connect(fitToWindowCheckBox, SIGNAL(clicked(bool)), this, SLOT(fitToWindow()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(goToFrame(int)));
    connect(framesSpeed, SIGNAL(valueChanged(int)), animated, SLOT(setSpeed(int)));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderPositionShow(int)));

    updateButtons();
    updateFrameSlider();

    QPropertyAnimation * animation;
    animation = new QPropertyAnimation(this, "size");
    animation->setEndValue(QSize(800, 600));
    animation->start();
}

void AnimationViewer::open(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load new Animation"),
                                                    currentAnimationDirectory);
    if(!fileName.isEmpty())
        loadFile(fileName);
}

void AnimationViewer::loadFile(const QString & fileName){
    if(QFileInfo(fileName).suffix() == "gif"){

        currentAnimationDirectory = QFileInfo(fileName).path();

        animated->stop();
        animatedLabel->setMovie(animated);
        animated->setFileName(fileName);
        animated->start();

        updateButtons();
        updateFrameSlider();
    }else
        QMessageBox::information(this, "Bad file Format",
                                 tr("It's don't look as animated image"));
}

void AnimationViewer::goToFrame(int frame){
    animated->jumpToFrame(frame);
}

void AnimationViewer::fitToWindow(){
    animatedLabel->setScaledContents(fitToWindowCheckBox->isChecked());
}

void AnimationViewer::createControls(){

    fitToWindowCheckBox = new QCheckBox(tr("Fit to Window"));

    slider = new QSlider(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(10);

    sliderLabel = new QLabel(tr("Frames"));

    framesSpeed = new QSpinBox;
    framesSpeed->setRange(1, 999);
    framesSpeed->setValue(100);
    framesSpeed->setSuffix(tr("%"));


    framesSpeedLabel = new QLabel(tr("Speed"));

    controlsLayout = new QGridLayout;

    controlsLayout->addWidget(fitToWindowCheckBox, 0, 0, 1, 2);
    controlsLayout->addWidget(sliderLabel, 1, 0);
    controlsLayout->addWidget(slider, 1, 1, 1, 2);
    controlsLayout->addWidget(framesSpeedLabel, 2, 0);
    controlsLayout->addWidget(framesSpeed, 2, 1, 1, 2);
}

void AnimationViewer::updateFrameSlider(){
    bool hasFrames = (animated->currentFrameNumber() >= 0);

    if(hasFrames){
        if(animated->frameCount() > 0)
            slider->setMaximum(animated->frameCount()-1);
        else
            if(animated->currentFrameNumber() > slider->maximum())
                slider->setMaximum(animated->currentFrameNumber());
        slider->setValue(animated->currentFrameNumber());
    }else
        slider->setValue(0);

    sliderLabel->setEnabled(hasFrames);
    slider->setEnabled(hasFrames);
}

void AnimationViewer::sliderPositionShow(int position){
    slider->setToolTip(QString::number(position));
}

void AnimationViewer::updateButtons(){
    playButton->setEnabled(animated->isValid() && animated->frameCount() >= 1
                           && animated->state() == QMovie::NotRunning);
    pauseButton->setEnabled(animated->state() != QMovie::NotRunning);
    pauseButton->setChecked(animated->state() == QMovie::Paused);
    stopButton->setEnabled(animated->state() != QMovie::NotRunning);
}

void AnimationViewer::createButtons(){
    QString appPath = QDir::currentPath() + "/icons";

    openFile = new QToolButton;
    openFile->setIcon(QIcon(appPath + "/open.png"));
    openFile->setWindowTitle(tr("&Open"));
    connect(openFile, SIGNAL(clicked(bool)), this, SLOT(open()));

    playButton = new QToolButton;
    playButton->setIcon(QIcon(appPath + "/play.png"));
    playButton->setWindowTitle(tr("&Play"));
    connect(playButton, SIGNAL(clicked(bool)), animated, SLOT(start()));

    pauseButton = new QToolButton;
    pauseButton->setIcon(QIcon(appPath + "/pause.png"));
    pauseButton->setCheckable(true);
    pauseButton->setWindowTitle(tr("&Pause"));
    connect(pauseButton, SIGNAL(clicked(bool)), animated, SLOT(setPaused(bool)));

    stopButton = new QToolButton;
    stopButton->setIcon(QIcon(appPath + "/stop.png"));
    stopButton->setWindowTitle(tr("&Stop"));
    connect(stopButton, SIGNAL(clicked(bool)), animated, SLOT(stop()));

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(openFile);
    buttonsLayout->addWidget(playButton);
    buttonsLayout->addWidget(pauseButton);
    buttonsLayout->addWidget(stopButton);
    buttonsLayout->addStretch();
}
