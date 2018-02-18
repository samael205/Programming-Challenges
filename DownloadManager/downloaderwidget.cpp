#include "downloaderwidget.h"
#include "newdownloaddialog.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableView>
#include <QProgressBar>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QListView>
#include <QLocale>

downloaderWidget::downloaderWidget(QWidget *parent)
    : QTabWidget(parent){
    downloadTable = new DownloaderTable;
    downloadPainter = new DownloaderItemDelegate;

    setupTable();

    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(downloadFinished(QNetworkReply*)));

    load();
}

downloaderWidget::~downloaderWidget(){
    save();
}
void downloaderWidget::setupTable(){
    proxy = new QSortFilterProxyModel(this);
    proxy->setSourceModel(downloadTable);
    proxy->setFilterKeyColumn(0);

    QTableView * dataViewer;
    dataViewer = new QTableView;
    dataViewer->setModel(proxy);
    dataViewer->setSortingEnabled(true);
    dataViewer->resizeColumnsToContents();
    dataViewer->setSelectionBehavior(QAbstractItemView::SelectRows);
    dataViewer->setSelectionMode(QAbstractItemView::SingleSelection);
    dataViewer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    dataViewer->setColumnWidth(0, 350);
    dataViewer->setColumnWidth(1, 150);
    dataViewer->setColumnWidth(2, 100);
    dataViewer->horizontalHeader()->setStretchLastSection(true);

    dataViewer->setItemDelegate(downloadPainter);

    connect(dataViewer, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(showDownloadedFileLocation(QModelIndex)));

    QIcon icon("icons/download.png");

    addTab(dataViewer, icon, "Data");
}

void downloaderWidget::Download(QUrl & url){
    QNetworkRequest request(url);
    currentDownload = manager.get(request);

    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));

    downloadTime.start();
}

void downloaderWidget::showDownloadedFileLocation(QModelIndex index){
    QString file = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath());
    QDesktopServices::openUrl(file);
}

void downloaderWidget::downloadProgress(qint64 bytesReceived, qint64 bytesTotal){
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;

    if(speed < 1024)
        unit = "bytes/sec";
    else if(speed < 1024 * 1024){
        speed /= 1024;
        unit = "kB/s";
    }else{
        speed /= (1024 * 1024);
        unit = "MB/s";
    }

    QString speedFormat;
    speedFormat.sprintf("%6.2f", speed);
    QString speedProgress = speedFormat + " " + unit;
    QString fileSizeBeforeDownload = sizeHuman(bytesTotal);
    qint64 progressPercent = (bytesReceived*100)/bytesTotal;

    QModelIndex index = downloadTable->index(0, 1, QModelIndex());
    downloadTable->setData(index, progressPercent, Qt::EditRole);

    index = downloadTable->index(0, 2, QModelIndex());
    downloadTable->setData(index, fileSizeBeforeDownload, Qt::EditRole);

    index = downloadTable->index(0, 3, QModelIndex());
    downloadTable->setData(index, speedProgress, Qt::EditRole);
}

QString downloaderWidget::saveFileName(QUrl & url){
    QString path = url.path();
    QString filename = QFileInfo(path).fileName();

    if(filename.isEmpty())
        filename = "download";

    int i = 0;
    if(QFile::exists(filename + QString::number(i))){
        while(QFile::exists(filename + QString::number(i)))
            ++i;
        filename += QString::number(i);
    }
    return filename;
}

bool downloaderWidget::saveToDisk(const QString & filename, QIODevice * data){
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    file.write(data->readAll());
    file.close();
    return true;
}

bool downloaderWidget::isHttpRedricted(QNetworkReply * reply){
    int httpCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return httpCode == 301 || httpCode == 302 || httpCode == 303
            || httpCode == 305 || httpCode == 307 || httpCode == 308;
}

QString downloaderWidget::getDownloadLink(){
    newDownloadDialog dialog;
    QString getLinkToData;

    if(dialog.exec())
        getLinkToData = dialog.linkToData->text();

    return getLinkToData;
}

void downloaderWidget::startDownloadProcess(){
    QUrl url = queueOfDownloads.dequeue();
    Download(url);
    downloadTime.start();
}

void downloaderWidget::remove(){
    QTableView * findSelected = static_cast<QTableView*>(currentWidget());
    QSortFilterProxyModel * proxySelected = static_cast<QSortFilterProxyModel*>(findSelected->model());
    QItemSelectionModel * selectionModel = findSelected->selectionModel();

    QModelIndexList indexies = selectionModel->selectedRows();

    foreach(QModelIndex index, indexies){
        int row = proxySelected->mapToSource(index).row();
        downloadTable->removeRows(row, 1, QModelIndex());
    }
}

void downloaderWidget::setDownload(){
    QString url = getDownloadLink();
    QString downloadFileName = QUrl(url).fileName();

    if(downloadTable->checkForDuplicateName(downloadFileName)){
        QMessageBox::information(this, "Duplicate data!",
                                 tr("There is %1 already in downloads!")
                                 .arg(downloadFileName));
        return;
    }

    if(!url.isEmpty()){
        queueOfDownloads.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
        downloadTable->insertRows(0, 1, QModelIndex());
        QModelIndex index = downloadTable->index(0, 0, QModelIndex());
        downloadTable->setData(index, downloadFileName, Qt::EditRole);
        startDownloadProcess();
    }
}

QString downloaderWidget::sizeHuman(qint64 fileSize){
    QStringList sizeExt;
    sizeExt << "KB" << "MB" << "GB" << "TB";

    QStringListIterator it(sizeExt);
    QString units("bytes");

    float num = fileSize;

    while(num >= 1024.0 && it.hasNext()){
        units = it.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 2) + " " + units;
}

void downloaderWidget::downloadFinished(QNetworkReply * reply){
    QUrl url = reply->url();
    if(!reply->error()){
        if(!isHttpRedricted(reply)){
            QString filename = saveFileName(url);
            if(saveToDisk(filename, reply)){
                QMessageBox::information(this, "Download successed!",
                                         tr("Download of %1 succesed!")
                                         .arg(filename));

        }else{
            downloadTable->removeRows(0, 1, QModelIndex());
            QMessageBox::information(this, "Http Redricted",
                                     tr("HTTP Error"));
       }
    }else{
        downloadTable->removeRows(0, 1, QModelIndex());
        QMessageBox::information(this, "Download Error",
                                 tr("Download error %1")
                                 .arg(url.errorString()));
        }
   }
}

void downloaderWidget::save(){
    QFile file("data.bin");
    if(file.open(QIODevice::WriteOnly)){
        QDataStream stream(&file);
        int n = downloadTable->rowCount(QModelIndex());
        int m = downloadTable->columnCount(QModelIndex());
        QModelIndex index;
        stream<<n<<m;
        REP(i, n)
                REP(j, m){
                        index = downloadTable->index(i, j, QModelIndex());
                        stream << downloadTable->data(index, Qt::DisplayRole);
        }
        file.close();
    }
}

void downloaderWidget::load(){
    QFile file("data.bin");
    if(file.open(QIODevice::ReadOnly)){
        QDataStream stream(&file);
        int n, m;
        stream >> n >> m;
        downloadTable->insertRows(0, n, QModelIndex());
        QModelIndex index;
        REP(i, n)
                REP(j, m){
                    index = downloadTable->index(i, j);
                    downloadTable->setData(index, stream, Qt::EditRole);
        }
    }
}
