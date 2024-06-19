#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "centercontrol.h"
#include "imageprovider.h"
#include "viewbridge.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    CenterControl &centerControl = CenterControl::instance();
    engine.rootContext()->setContextProperty("viewbridge", centerControl.viewBridge());
    engine.addImageProvider(QLatin1String("img"), centerControl.viewBridge()->getImageProvider());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("../../Main.qml")));

    return app.exec();
}
