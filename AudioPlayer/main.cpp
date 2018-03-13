#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLContext>
#include <QtQml/QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL){
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);

    QQuickView view;
    view.setFormat(format);
    view.create();

    view.setSource(QUrl("qrc:/main.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setMaximumSize(QSize(1820, 1080));
    view.setMinimumSize(QSize(300, 150));
    view.show();

    return app.exec();
}
