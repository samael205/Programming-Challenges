#include <QApplication>
#include "animationviewer.h"

int main(int argv, char **argc){
    QApplication app(argv, argc);

    AnimationViewer viewer;
    viewer.show();

    return app.exec();
}
