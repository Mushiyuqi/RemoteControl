#include "viewbridge.h"
#include <QGuiApplication>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include "centercontrol.h"
#include "imageprovider.h"
#include "pevent.h"
#include "viewcontrol.h"

ViewBridge::ViewBridge(QObject *parent)
    : QObject{parent}
{
    _cctrl = qobject_cast<CenterControl *>(parent);
    m_imageprovider = new ImageProvider;
}

ViewBridge::~ViewBridge(){}

ImageProvider *ViewBridge::getImageProvider()
{
    return m_imageprovider;
}

int ViewBridge::getImgWidth()
{
    return m_imageprovider->m_width;
}

int ViewBridge::getImgHeight()
{
    return m_imageprovider->m_height;
}

//更新图片源
void ViewBridge::updatePixmap(QPixmap pixmap)
{
    // pixmap = pixmap.scaled(400, 300);
    m_imageprovider->setPixmap(pixmap);
    emit needUpdate();
}

void ViewBridge::setViewControl(ViewControl *vctrl)
{
    _vctrl = vctrl;
}

//处理ip,port是否连接成功，成功则返回true
bool ViewBridge::handleLink(QString textIP, QString textPort)
{
    return _cctrl->linkPc(textIP, textPort.toUShort());
}

void ViewBridge::handleCloseShare()
{
    _cctrl->closeSharePc();
}

void ViewBridge::handleClientClose()
{
    if (_vctrl == nullptr)
        return;
    _vctrl->closeConnect();
}

void ViewBridge::handlerShare()
{
    _cctrl->sharePc();
}

void ViewBridge::mouseTappedEvent(
    const int x, const int y, const int width, const int height, const int type)
{
    if (_vctrl == nullptr)
        return;

    //设置eventNode类型
    EventNode pNode(x, y, (double) x / width, (double) y / height);
    switch (type) {
    case Qt::LeftButton:
        pNode.m_type = EventNode::Type::mouseLeftClick;
        break;
    case Qt::RightButton:
        pNode.m_type = EventNode::Type::mouseRightClick;
        break;
    }

    //设置执行事件
    _vctrl->eventAction(pNode);
}

void ViewBridge::mouseMoveEvent(const int x, const int y, const int width, const int height)
{
    if (_vctrl == nullptr)
        return;

    //设置eventNode类型
    EventNode pNode(x, y, (double) x / width, (double) y / height, EventNode::Type::mouseMove);

    //设置执行事件
    _vctrl->eventAction(pNode);
}
