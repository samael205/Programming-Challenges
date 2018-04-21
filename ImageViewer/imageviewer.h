#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtWidgets>
#include <QMainWindow>

#ifndef QT_NO_PRINTER
    #include <QtPrintSupport/QPrinter>
#endif

#define REP(i, n) for(int i=0; i<n; i++)

class ImageViewer : public QMainWindow{
    Q_OBJECT
public:
    ImageViewer();
    ~ImageViewer();
    bool loadImage(const QString&);
private slots:
    void openFile();
    void openFiles();
    void saveAs();
    void setNormalSize();
    void fitImage();
    void zoomIn();
    void zoomOut();
    void copy();
    void paste();
    void print();
    void emptyShow();
    void deleteSelectedIcon();
    void ChangeThumbnailVisible();
    void changeImageByIconSelect();
    void quit();
private:
    QListWidget * iconsShow;
    bool iconExist(const QString&);

    QLabel * imageShow;
    QImage image;
    void setImage(const QImage&);
    void setAnimatedImage(QMovie*);

    void scaleImage(double);
    void adjustScrollBar(QScrollBar*, double);

#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
    bool saveImage(const QString&);

    QScrollArea * scroll;
    double scale;

    void setActions();
    void updateActions();

    QMenu * fileMenu;
    QAction * open;
    QAction * openMultiple;
    QAction * save;
    QAction * close;
    QAction * clearImage;
    QAction * printAct;
    QAction * exit;

    QMenu * editMenu;
    QAction * copyThis;
    QAction * pasteThis;

    QMenu * viewMenu;
    QAction * setZoomIn;
    QAction * setZoomOut;
    QAction * normalSize;
    QAction * fitToWindow;
    QAction * ThumbnailShow;
};

#endif // IMAGEVIEWER_H
