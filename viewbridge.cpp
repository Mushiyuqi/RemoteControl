#include "viewbridge.h"
#include <QGuiApplication>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include "imageprovider.h"

ViewBridge::ViewBridge(QObject *parent)
    : QObject{parent}
{
    m_imageprovider = new ImageProvider;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        updatePixmap();
        emit needUpdate();
    });
    timer->start(4000);
}

//更新图片源
void ViewBridge::updatePixmap()
{
    qDebug() << "调用updatePixmap，更新图片源";
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    // pixmap = pixmap.scaled(400, 300);
    m_imageprovider->setPixmap(pixmap);
}

//处理ip,port是否连接成功，成功则返回true
bool ViewBridge::handleValidText(const QString &textIP, const QString &textPort)
{
    qDebug() << "valid text ";
    //todo：后端处理text
    return true;
}

bool ViewBridge::handlerValidShare()
{
    return true;
}
