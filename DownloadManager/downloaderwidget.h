#ifndef DOWNLOADERWIDGET_H
#define DOWNLOADERWIDGET_H

#include <QTabWidget>
#include <QTime>
#include <QSortFilterProxyModel>
#include <QQueue>

#include "downloadertable.h"
#include "downloaderitemdelegate.h"
#include "downloadprogresssingleconnect.h"

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
    void showDownloadedFileLocation();
    void remove();
private:
    void setupTable();
    DownloaderTable * downloadTable;
    DownloaderItemDelegate * downloadPainter;
    QSortFilterProxyModel * proxy;

    QQueue<QUrl> queueOfDownloads;
    QVector<DownloadProgressSingleConnect*> downloads;
};

#endif // DOWNLOADERWIDGET_H
