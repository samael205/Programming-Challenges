#ifndef DOWNLOADERWIDGET_H
#define DOWNLOADERWIDGET_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkReply>
#include <QTabWidget>
#include <QTime>
#include <QSortFilterProxyModel>
#include <QQueue>

#include "downloadertable.h"
#include "downloaderitemdelegate.h"

class downloaderWidget : public QTabWidget
{
    Q_OBJECT
public:
    downloaderWidget(QWidget * parent = 0);
    ~downloaderWidget();

    void Download(QUrl &);
    static QString saveFileName(QUrl&);
    bool saveToDisk(const QString&, QIODevice*);
    static bool isHttpRedricted(QNetworkReply*);

    QString getDownloadLink();
    void setDownload();
    void startDownloadProcess();

    void save();
    void load();
public slots:
    void downloadFinished(QNetworkReply*);
    void downloadProgress(qint64,qint64);
    void showDownloadedFileLocation(QModelIndex);
    void remove();
private:
    void setupTable();
    DownloaderTable * downloadTable;
    DownloaderItemDelegate * downloadPainter;
    QSortFilterProxyModel * proxy;

    QNetworkAccessManager manager;
    QNetworkReply *  currentDownload = nullptr;

    QQueue<QUrl> queueOfDownloads;
    QTime downloadTime;
protected:
    QString sizeHuman(const qint64);
};

#endif // DOWNLOADERWIDGET_H
