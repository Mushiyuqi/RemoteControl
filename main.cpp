#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "centercontrol.h"
#include "imageprovider.h"
#include "viewbridge.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    CenterControl::instance().show();

    ViewBridge *viewbridge = new ViewBridge;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", viewbridge);
    engine.addImageProvider(QLatin1String("img"), viewbridge->getImageProvider());

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    // engine.loadFromModule("../../RemoteControl", "Main");
    engine.load(QUrl(QStringLiteral("../../Main.qml")));

    return app.exec();
}
