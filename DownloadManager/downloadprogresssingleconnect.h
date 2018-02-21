#ifndef DOWNLOADPROGRESSSINGLECONNECT_H
#define DOWNLOADPROGRESSSINGLECONNECT_H

#include "downloadertable.h"
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QObject>
#include <QTime>
#include <QUrl>

class DownloadProgressSingleConnect : public QObject
{
    Q_OBJECT
public:
    DownloadProgressSingleConnect(QUrl,DownloaderTable*);

    const QUrl & getUrl() const;
signals:
    void downloadFinished(QNetworkReply*);
public slots:
      void downloadProgress(qint64,qint64);
private:
    QString size;
    DownloaderTable * dataModel;
    QTime downloadTime;
    QUrl url;
protected:
    QString sizeHuman(const qint64);
};

#endif // DOWNLOADPROGRESSSINGLECONNECT_H
