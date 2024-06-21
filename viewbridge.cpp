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

void ViewBridge::mouseLeftReleaseEvent(int x, int y, int width, int height)
{
    if (_vctrl == nullptr)
        return;
    PositionNode pNode(x,
                       y,
                       (double) x / width,
                       (double) y / height,
                       PositionNode::Type::mouseLeftRelease);
    _vctrl->mouseClickedAcction(pNode);
}

void ViewBridge::mouseRightReleaseEvent(int x, int y, int width, int height)
{
    if (_vctrl == nullptr)
        return;
    PositionNode pNode(x,
                       y,
                       (double) x / width,
                       (double) y / height,
                       PositionNode::Type::mouseRightRelease);
    _vctrl->mouseClickedAcction(pNode);
}

void ViewBridge::mouseMoveEvent(int x, int y, int width, int height)
{
    if (_vctrl == nullptr)
        return;
    PositionNode pNode(x, y, (double) x / width, (double) y / height, PositionNode::Type::mouseMove);
    _vctrl->mouseMoveAcction(pNode);
}
