#include "threadserver.h"

ThreadServer::ThreadServer(QObject * parent)
    : QTcpServer(parent){

    quotesList << tr("“Be yourself; everyone else is already taken.” ")
               << tr("“A room without books is like a body without a soul.”")
               << tr("“Without music, life would be a mistake.” ")
               << tr("“I am enough of an artist to draw freely upon my imagination."
                     "\nImagination is more important than knowledge.\nKnowledge is limited."
                     "\nImagination encircles the world.” ")
               << tr("“The difference between genius and stupidity is: genius has its limits.”")
               << tr("“If you're going through hell, keep going.”");
}

void ThreadServer::incomingConnection(qintptr socketDescriptor){
    QString quote = quotesList[qrand()%quotesList.size()];
    ThreadQuote * thread = new ThreadQuote(socketDescriptor, quote, this);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
