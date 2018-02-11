#ifndef ANIMATIONVIEWER_H
#define ANIMATIONVIEWER_H

#include <QtWidgets>

class AnimationViewer : public QWidget
{
    Q_OBJECT
public:
    AnimationViewer(QWidget * parent = 0);
    void loadFile(const QString&);
public slots:
   void open();
   void goToFrame(int);
   void fitToWindow();
   void updateFrameSlider();
   void updateButtons();
   void sliderPositionShow(int);
private:
    QVBoxLayout * mainLayout;

    void createControls();
    void createButtons();

    QMovie * animated;
    QLabel * animatedLabel;
    QString currentAnimationDirectory;

    QGridLayout * controlsLayout;
    QCheckBox * fitToWindowCheckBox;

    QHBoxLayout * buttonsLayout;

    QSlider * slider;
    QLabel * sliderLabel;

    QDialog * framesCountDialog;

    QSpinBox * framesSpeed;
    QLabel * framesSpeedLabel;

    QToolButton * openFile;
    QToolButton * playButton;
    QToolButton * pauseButton;
    QToolButton * stopButton;



};

#endif // ANIMATIONVIEWER_H
