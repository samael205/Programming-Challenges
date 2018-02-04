#include "imageviewer.h"

ImageViewer::ImageViewer()
{
    imageShow = new QLabel;
    imageShow->setBackgroundRole(QPalette::Base);
    imageShow->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageShow->setScaledContents(true);

    scroll = new QScrollArea;
    scroll->setBackgroundRole(QPalette::Dark);
    scroll->setWidget(imageShow);
    scroll->setVisible(false);

    scale = 1;

    setCentralWidget(new QWidget);

    QGridLayout * mainLayout;
    mainLayout = new QGridLayout;
    mainLayout->addWidget(scroll, 0, 1);

    iconsShow = new QListWidget;
    iconsShow->setViewMode(QListWidget::IconMode);
    iconsShow->setIconSize(QSize(80, 80));
    iconsShow->setFixedWidth(97);
    iconsShow->setVisible(false);
    iconsShow->setEnabled(true);
    iconsShow->setStyleSheet("font-size: 1px; color: transparent; white-space: nowrap;");

    mainLayout->addWidget(iconsShow, 0, 0, Qt::AlignLeft);
    centralWidget()->setLayout(mainLayout);

    setActions();

    connect(iconsShow, SIGNAL(clicked(QModelIndex)), this, SLOT(changeImageByIconSelect()));

    QPropertyAnimation * animation;
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(50);

    QRect screenSize = QApplication::desktop()->screenGeometry();
    int x = screenSize.width();
    int y = screenSize.height();

    animation->setEndValue(QRect(x/2-300, y/2-300, 1000, 700));
    animation->start();
}

ImageViewer::~ImageViewer(){

}

void ImageViewer::setActions(){

    fileMenu = menuBar()->addMenu("File");

    clearImage = new QAction("New", this);
    fileMenu->addAction(clearImage);
    clearImage->setShortcut(QKeySequence::New);
    connect(clearImage, SIGNAL(triggered(bool)), this, SLOT(emptyShow()));;

    fileMenu->addSeparator();

    open = new QAction("Open", this);
    fileMenu->addAction(open);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(openFile()));

    openMultiple = new QAction("Open Files", this);
    fileMenu->addAction(openMultiple);
    connect(openMultiple, SIGNAL(triggered(bool)), this, SLOT(openFiles()));

    save = new QAction("Save", this);
    fileMenu->addAction(save);
    connect(save, SIGNAL(triggered(bool)), this, SLOT(saveAs()));

    close = new QAction("Close", this);
    fileMenu->addAction(close);
    close->setShortcut(QKeySequence::Close);
    connect(close, SIGNAL(triggered(bool)), imageShow, SLOT(clear()));

    fileMenu->addSeparator();
    exit = new QAction("Exit", this);
    fileMenu->addAction(exit);
    exit->setShortcut(QKeySequence::Quit);
    connect(exit, SIGNAL(triggered(bool)), this, SLOT(quit()));

    QAction * deleteByIcon;
    deleteByIcon = new QAction(iconsShow);
    deleteByIcon->setShortcut(QKeySequence::Delete);
    iconsShow->addAction(deleteByIcon);
    connect(deleteByIcon, SIGNAL(triggered(bool)), this, SLOT(deleteSelectedIcon()));

    editMenu = menuBar()->addMenu("Edit");

    copyThis = new QAction("Copy", this);
    editMenu->addAction(copyThis);
    copyThis->setShortcut(QKeySequence::Copy);
    connect(copyThis, SIGNAL(triggered(bool)), this, SLOT(copy()));

    pasteThis = new QAction("Paste", this);
    editMenu->addAction(pasteThis);
    pasteThis->setShortcut(QKeySequence::Paste);
    connect(pasteThis, SIGNAL(triggered(bool)), this, SLOT(paste()));

    viewMenu = menuBar()->addMenu("View");

    setZoomIn = new QAction("Zoom In", this);
    viewMenu->addAction(setZoomIn);
    connect(setZoomIn, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    setZoomIn->setShortcut(QKeySequence::ZoomIn);
    setZoomIn->setEnabled(false);

    setZoomOut = new QAction("Zoom Out", this);
    viewMenu->addAction(setZoomOut);
    connect(setZoomOut, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    setZoomOut->setShortcut(QKeySequence::ZoomOut);
    setZoomOut->setEnabled(false);

    normalSize = new QAction("Normal Size", this);
    viewMenu->addAction(normalSize);
    connect(normalSize, SIGNAL(triggered(bool)), this, SLOT(setNormalSize()));
    normalSize->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindow = new QAction("Fit to Window", this);
    viewMenu->addAction(fitToWindow);
    connect(fitToWindow, SIGNAL(triggered(bool)), this, SLOT(fitImage()));
    fitToWindow->setEnabled(false);
    fitToWindow->setCheckable(true);
    fitToWindow->setShortcut(tr("CTRL+F"));

    ThumbnailShow = new QAction("Thumbnail", this);
    viewMenu->addAction(ThumbnailShow);
    connect(ThumbnailShow, SIGNAL(triggered(bool)), this, SLOT(ChangeThumbnailVisible()));
    ThumbnailShow->setCheckable(true);
    ThumbnailShow->setChecked(false);
}

void ImageViewer::setImage(const QImage & getImg){
    image = getImg;
    imageShow->setPixmap(QPixmap::fromImage(image));

    scroll->setVisible(true);
    fitToWindow->setEnabled(true);
    updateActions();
}

void ImageViewer::changeImageByIconSelect(){
    auto currentIcons = iconsShow->selectedItems();
    if(!currentIcons.empty()){
        QString selectedImg = currentIcons.last()->text();
        loadImage(selectedImg);
    }
}

bool ImageViewer::iconExist(const QString & imgToCheck){
    REP(i, iconsShow->count())
            if(iconsShow->item(i)->data(Qt::DisplayRole).toString() == imgToCheck){
                    iconsShow->setCurrentRow(i);
                    return true;
                 }
    return false;
}

bool ImageViewer::loadImage(const QString & fileName){
    QImageReader reader(fileName);
    const QImage img = reader.read();
    if(img.isNull()){
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Can't load : %1").arg(QDir::toNativeSeparators(fileName),
                                                           reader.errorString()));
        return false;
    }
    setImage(img);

    if(!iconExist(fileName)){
        QListWidgetItem * setIcon;
        setIcon = new QListWidgetItem(QIcon(fileName), fileName);
        iconsShow->addItem(setIcon);
        iconsShow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    setWindowFilePath(fileName);

    if(!fitToWindow->isChecked())
           imageShow->adjustSize();

    return true;
}

void ImageViewer::quit(){
    QGuiApplication::exit();
}

static void initializeImageFileDialog(QFileDialog & dialog, QFileDialog::AcceptMode acceptMode){
    static bool first = true;

    if(first){
        first = false;
        const QStringList picturesLocation = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocation.isEmpty() ? QDir::currentPath() : picturesLocation.last());
    }

    const QByteArrayList supportedMime = (acceptMode == QFileDialog::AcceptOpen)
            ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();

    QStringList mimeTypes;
    foreach(const QByteArray & mimeType, supportedMime)
        mimeTypes.append(mimeType);

    dialog.setMimeTypeFilters(mimeTypes);
    dialog.selectMimeTypeFilter("image/jpeg");

    if(acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void ImageViewer::openFile(){
    QFileDialog setImage(this, tr("Open File"));
    initializeImageFileDialog(setImage, QFileDialog::AcceptOpen);

    while(setImage.exec() == QDialog::Accepted && !loadImage(setImage.selectedFiles().first())){
        }
}

void ImageViewer::openFiles(){
    QFileDialog getImages;
    QStringList imagesPaths;

    initializeImageFileDialog(getImages, QFileDialog::AcceptOpen);
    imagesPaths = getImages.getOpenFileNames();

    if(imagesPaths.size() > 1){
        ThumbnailShow->setChecked(true);
        ChangeThumbnailVisible();
    }

    if(!imagesPaths.empty())
        REP(i, imagesPaths.length())
                loadImage(imagesPaths[i]);
}

bool ImageViewer::saveImage(const QString & fileName){
   QImageWriter saveFile(fileName);

   if(!saveFile.write(image))
       QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                tr("Cannot write %1").arg(QDir::toNativeSeparators(fileName)));

   const QString msg = tr("Wrote %1").arg(QDir::toNativeSeparators(fileName));
   statusBar()->showMessage(msg);

   return true;
}

void ImageViewer::saveAs(){
    QFileDialog saveFile(this, "Save File...");
     initializeImageFileDialog(saveFile, QFileDialog::AcceptSave);

    while(saveFile.exec() == QDialog::Accepted && !saveImage(saveFile.selectedFiles().first())){
        }
}

void ImageViewer::zoomIn(){
   scaleImage(1.25);
}

void ImageViewer::zoomOut(){
   scaleImage(0.75);
}

void ImageViewer::emptyShow(){
   imageShow->clear();
   iconsShow->clear();
}

void ImageViewer::deleteSelectedIcon(){
    auto selectedIcons = iconsShow->selectedItems();

    foreach(auto * icon, selectedIcons)
        delete iconsShow->takeItem(iconsShow->row(icon));
}

void ImageViewer::copy(){
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboard(){
    if(const QMimeData * mime = QGuiApplication::clipboard()->mimeData())
        if(mime->hasImage()){
            const QImage currentImg = qvariant_cast<QImage>(mime->imageData());
            if(!currentImg.isNull())
                return currentImg;
        }
    return QImage();
}
#endif

void ImageViewer::paste(){
#ifndef QT_NO_CLIPBOARD
    const QImage newImg = clipboard();
    if(newImg.isNull())
        statusBar()->showMessage(tr("No current Image in clipboard!"));
    else{
        setImage(newImg);
        setWindowFilePath(QString());
        const QString msg = tr("Obtained image from clipboard %1x%2, Depth: %3")
                .arg(newImg.width()).arg(newImg.height()).arg(newImg.depth());
        statusBar()->showMessage(msg);
    }
#endif
}

void ImageViewer::setNormalSize(){
    imageShow->adjustSize();
    scale = 1.0;
}

void ImageViewer::fitImage(){
    bool fit = fitToWindow->isChecked();
    scroll->setWidgetResizable(fit);
    if(!fit)
        setNormalSize();
    updateActions();
}

void ImageViewer::adjustScrollBar(QScrollBar * scrollBar, double factory){
    scrollBar->setValue(int(factory * scrollBar->value()
                            + ((factory - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::updateActions(){
    setZoomIn->setEnabled(!fitToWindow->isChecked());
    setZoomOut->setEnabled(!fitToWindow->isChecked());
    normalSize->setEnabled(!fitToWindow->isChecked());
}

void ImageViewer::scaleImage(double scaleValue){
    Q_ASSERT(imageShow->pixmap());

    scale *= scaleValue;
    imageShow->resize(scale * imageShow->pixmap()->size());

    adjustScrollBar(scroll->horizontalScrollBar(), scale);
    adjustScrollBar(scroll->verticalScrollBar(), scale);
}

void ImageViewer::ChangeThumbnailVisible(){
   bool isIconEnabled =  ThumbnailShow->isChecked();
   iconsShow->setVisible(isIconEnabled);
}


