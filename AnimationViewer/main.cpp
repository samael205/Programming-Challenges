#include <QApplication>
#include <QIcon>
#include "animationviewer.h"

int main(int argv, char **argc){
    QApplication app(argv, argc);
    Q_INIT_RESOURCE(content);
    app.setWindowIcon(QIcon(":/icons/icons/icon.png"));

    AnimationViewer viewer;
    viewer.show();

    return app.exec();
}
