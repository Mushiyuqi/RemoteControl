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
}

ViewBridge::~ViewBridge()
{
    delete m_imageprovider;
}

//更新图片源
void ViewBridge::updatePixmap(QPixmap pixmap)
{
    // pixmap = pixmap.scaled(400, 300);
    m_imageprovider->setPixmap(pixmap);
    emit needUpdate();
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
