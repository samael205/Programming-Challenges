#include "chatdialog.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(content);
    app.setWindowIcon(QIcon(":/icons/icons/chat.png"));
    ChatDialog chat;
    chat.show();

    return app.exec();
}
