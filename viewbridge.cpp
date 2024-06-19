#include "viewbridge.h"
#include <QGuiApplication>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include "centercontrol.h"
#include "imageprovider.h"

ViewBridge::ViewBridge(QObject *parent)
    : QObject{parent}
{
    _cctrl = qobject_cast<CenterControl *>(parent);
    m_imageprovider = new ImageProvider;
    QTimer *timer = new QTimer(this);
}

ViewBridge::~ViewBridge()
{
    delete m_imageprovider;
}

int ViewBridge::getImgWidth()
{
    return m_imageprovider->_width;
}

int ViewBridge::getImgHeight()
{
    return m_imageprovider->_height;
}
//更新图片源
void ViewBridge::updatePixmap(QPixmap pixmap)
{
    // pixmap = pixmap.scaled(400, 300);
    m_imageprovider->setPixmap(pixmap);
    emit needUpdate();
}

//处理ip,port是否连接成功，成功则返回true
bool ViewBridge::handleValidLink(QString textIP, QString textPort)
{
    qDebug() << "valid text ";
    _cctrl->linkPc(textIP, textPort.toUShort());
    //todo：后端处理text
    return true;
}

void ViewBridge::handleUnShare()
{
    _cctrl->closeSharePc();
}

bool ViewBridge::handlerValidShare()
{
    _cctrl->sharePc();
    return true;
}
