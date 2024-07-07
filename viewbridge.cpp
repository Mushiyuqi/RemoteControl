#include "viewbridge.h"
#include <QGuiApplication>
#include <QScreen>
#include <QThread>
#include <QTimer>
#include "centercontrol.h"
#include "imageprovider.h"
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
    m_imageprovider->setPixmap(pixmap);
    m_imageprovider->m_width = pixmap.width();
    m_imageprovider->m_height = pixmap.height();
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
    EventNode eNode(x, y, (double) x / width, (double) y / height);
    switch (type) {
    case Qt::LeftButton:
        eNode.m_type = EventNode::Type::mouseLeftClick;
        break;
    case Qt::RightButton:
        eNode.m_type = EventNode::Type::mouseRightClick;
        break;
    }

    //设置执行事件
    _vctrl->eventAction(eNode);
}
void ViewBridge::mouseDoubleTappedEvent(const int x, const int y, const int width, const int height)
{
    if (_vctrl == nullptr)
        return;

    //设置eventNode类型
    EventNode eNode(x, y, (double) x / width, (double) y / height, EventNode::Type::mouseDouble);

    _vctrl->eventAction(eNode);
}
void ViewBridge::mouseMoveEvent(const int x, const int y, const int width, const int height)
{
    if (_vctrl == nullptr)
        return;

    //设置eventNode类型
    EventNode eNode(x, y, (double) x / width, (double) y / height, EventNode::Type::mouseMove);

    //设置执行事件
    _vctrl->eventAction(eNode);
}
void ViewBridge::mouseWheelEvent(
    const int x, const int y, const int width, const int height, int roolLength)
{
    if (_vctrl == nullptr)
        return;

    EventNode eNode(x, y, (double) x / width, (double) y / height);
    //负值向上
    eNode.m_type = roolLength < 0 ? EventNode::Type::mouseScrollUp
                                  : EventNode::Type::mouseScrollDown;

    qDebug() << "roolLength: " << roolLength;
    _vctrl->eventAction(eNode);
}
void ViewBridge::keyPressEvent(int keyType)
{
    if (_vctrl == nullptr)
        return;

    //qDebug() << "ViewBridge::keyPressEvent: is pressed";

    EventNode eNode(0, 0, 0, 0, EventNode::Type::keyPress);
    if (!getKetType(eNode, keyType)) {
        return;
    }

    _vctrl->eventAction(eNode);
    //qDebug() << "eventAction is used";
}
void ViewBridge::keyReleaseEvent(int keyType)
{
    if (_vctrl == nullptr)
        return;

    EventNode eNode(0, 0, 0, 0, EventNode::Type::keyRelease);
    if (!getKetType(eNode, keyType)) {
        return;
    }
    _vctrl->eventAction(eNode);
}
bool ViewBridge::getKetType(EventNode &eNode, int keyType)
{
    bool flag = true;
    switch (keyType) {
    case Qt::Key_A:
        eNode.m_keyType = EventNode::KeyType::key_A;
        break;
    case Qt::Key_B:
        eNode.m_keyType = EventNode::KeyType::key_B;
        break;
    case Qt::Key_C:
        eNode.m_keyType = EventNode::KeyType::key_C;
        break;
    case Qt::Key_D:
        eNode.m_keyType = EventNode::KeyType::key_D;
        break;
    case Qt::Key_E:
        eNode.m_keyType = EventNode::KeyType::key_E;
        break;
    case Qt::Key_F:
        eNode.m_keyType = EventNode::KeyType::key_F;
        break;
    case Qt::Key_G:
        eNode.m_keyType = EventNode::KeyType::key_G;
        break;
    case Qt::Key_H:
        eNode.m_keyType = EventNode::KeyType::key_H;
        break;
    case Qt::Key_I:
        eNode.m_keyType = EventNode::KeyType::key_I;
        break;
    case Qt::Key_J:
        eNode.m_keyType = EventNode::KeyType::key_J;
        break;
    case Qt::Key_K:
        eNode.m_keyType = EventNode::KeyType::key_K;
        break;
    case Qt::Key_L:
        eNode.m_keyType = EventNode::KeyType::key_L;
        break;
    case Qt::Key_M:
        eNode.m_keyType = EventNode::KeyType::key_M;
        break;
    case Qt::Key_N:
        eNode.m_keyType = EventNode::KeyType::key_N;
        break;
    case Qt::Key_O:
        eNode.m_keyType = EventNode::KeyType::key_O;
        break;
    case Qt::Key_P:
        eNode.m_keyType = EventNode::KeyType::key_P;
        break;
    case Qt::Key_Q:
        eNode.m_keyType = EventNode::KeyType::key_Q;
        break;
    case Qt::Key_R:
        eNode.m_keyType = EventNode::KeyType::key_R;
        break;
    case Qt::Key_S:
        eNode.m_keyType = EventNode::KeyType::key_S;
        break;
    case Qt::Key_T:
        eNode.m_keyType = EventNode::KeyType::key_T;
        break;
    case Qt::Key_U:
        eNode.m_keyType = EventNode::KeyType::key_U;
        break;
    case Qt::Key_V:
        eNode.m_keyType = EventNode::KeyType::key_V;
        break;
    case Qt::Key_W:
        eNode.m_keyType = EventNode::KeyType::key_W;
        break;
    case Qt::Key_X:
        eNode.m_keyType = EventNode::KeyType::key_X;
        break;
    case Qt::Key_Y:
        eNode.m_keyType = EventNode::KeyType::key_Y;
        break;
    case Qt::Key_Z:
        eNode.m_keyType = EventNode::KeyType::key_Z;
        break;
    case Qt::Key_QuoteLeft:
        eNode.m_keyType = EventNode::KeyType::key_QuoteLeft;
        break;
    case Qt::Key_0:
        eNode.m_keyType = EventNode::KeyType::key_0;
        break;
    case Qt::Key_1:
        eNode.m_keyType = EventNode::KeyType::key_1;
        break;
    case Qt::Key_2:
        eNode.m_keyType = EventNode::KeyType::key_2;
        break;
    case Qt::Key_3:
        eNode.m_keyType = EventNode::KeyType::key_3;
        break;
    case Qt::Key_4:
        eNode.m_keyType = EventNode::KeyType::key_4;
        break;
    case Qt::Key_5:
        eNode.m_keyType = EventNode::KeyType::key_5;
        break;
    case Qt::Key_6:
        eNode.m_keyType = EventNode::KeyType::key_6;
        break;
    case Qt::Key_7:
        eNode.m_keyType = EventNode::KeyType::key_7;
        break;
    case Qt::Key_8:
        eNode.m_keyType = EventNode::KeyType::key_8;
        break;
    case Qt::Key_9:
        eNode.m_keyType = EventNode::KeyType::key_9;
        break;
    case Qt::Key_Minus:
        eNode.m_keyType = EventNode::KeyType::key_Minus;
        break;
    case Qt::Key_Equal:
        eNode.m_keyType = EventNode::KeyType::key_Equal;
        break;
    case Qt::Key_Backspace:
        eNode.m_keyType = EventNode::KeyType::key_Backspace;
        break;
    case Qt::Key_BracketLeft:
        eNode.m_keyType = EventNode::KeyType::key_BracketLeft;
        break;
    case Qt::Key_BracketRight:
        eNode.m_keyType = EventNode::KeyType::key_BracketRight;
        break;
    case Qt::Key_Backslash:
        eNode.m_keyType = EventNode::KeyType::key_Backslash;
        break;
    case Qt::Key_Return:
        eNode.m_keyType = EventNode::KeyType::key_Return;
        break;
    case Qt::Key_Tab:
        eNode.m_keyType = EventNode::KeyType::key_Tab;
        break;
    case Qt::Key_CapsLock:
        eNode.m_keyType = EventNode::KeyType::key_CapsLock;
        break;
    case Qt::Key_Shift:
        eNode.m_keyType = EventNode::KeyType::key_Shift;
        break;
    case Qt::Key_Control:
        eNode.m_keyType = EventNode::KeyType::key_Ctrl;
        break;
    case Qt::Key_Meta:
        eNode.m_keyType = EventNode::KeyType::key_Meta;
        break;
    case Qt::Key_Alt:
        eNode.m_keyType = EventNode::KeyType::key_Alt;
        break;
    case Qt::Key_Space:
        eNode.m_keyType = EventNode::KeyType::key_Space;
        break;
    case Qt::Key_Semicolon:
        eNode.m_keyType = EventNode::KeyType::key_Semicolon;
        break;
    case Qt::Key_Apostrophe:
        eNode.m_keyType = EventNode::KeyType::key_Apostrophe;
        break;
    case Qt::Key_Comma:
        eNode.m_keyType = EventNode::KeyType::key_Comma;
        break;
    case Qt::Key_Period:
        eNode.m_keyType = EventNode::KeyType::key_Period;
        break;
    case Qt::Key_Slash:
        eNode.m_keyType = EventNode::KeyType::key_Slash;
        break;
    case Qt::Key_Escape:
        eNode.m_keyType = EventNode::KeyType::key_Esc;
        break;
    case Qt::Key_ScrollLock:
        eNode.m_keyType = EventNode::KeyType::key_ScrollLock;
        break;
    case Qt::Key_Print:
        eNode.m_keyType = EventNode::KeyType::key_Print;
        break;
    case Qt::Key_Pause:
        eNode.m_keyType = EventNode::KeyType::key_PauseBreak;
        break;
    case Qt::Key_Insert:
        eNode.m_keyType = EventNode::KeyType::key_Insert;
        break;
    case Qt::Key_Home:
        eNode.m_keyType = EventNode::KeyType::key_Home;
        break;
    case Qt::Key_PageUp:
        eNode.m_keyType = EventNode::KeyType::key_PageUp;
        break;
    case Qt::Key_PageDown:
        eNode.m_keyType = EventNode::KeyType::key_PageDown;
        break;
    case Qt::Key_Delete:
        eNode.m_keyType = EventNode::KeyType::key_Delete;
        break;
    case Qt::Key_End:
        eNode.m_keyType = EventNode::KeyType::key_End;
        break;
    case Qt::Key_Up:
        eNode.m_keyType = EventNode::KeyType::key_Up;
        break;
    case Qt::Key_Down:
        eNode.m_keyType = EventNode::KeyType::key_Down;
        break;
    case Qt::Key_Left:
        eNode.m_keyType = EventNode::KeyType::key_Left;
        break;
    case Qt::Key_Right:
        eNode.m_keyType = EventNode::KeyType::key_Right;
        break;
    case Qt::Key_Exclam:
        eNode.m_keyType = EventNode::KeyType::key_Exclam;
        break;
    case Qt::Key_At:
        eNode.m_keyType = EventNode::KeyType::key_At;
        break;
    case Qt::Key_NumberSign:
        eNode.m_keyType = EventNode::KeyType::key_NumberSign;
        break;
    case Qt::Key_Dollar:
        eNode.m_keyType = EventNode::KeyType::key_Dollar;
        break;
    case Qt::Key_Percent:
        eNode.m_keyType = EventNode::KeyType::key_Percent;
        break;
    case Qt::Key_AsciiCircum:
        eNode.m_keyType = EventNode::KeyType::key_AsciiCircum;
        break;
    case Qt::Key_Ampersand:
        eNode.m_keyType = EventNode::KeyType::key_Ampersand;
        break;
    case Qt::Key_Asterisk:
        eNode.m_keyType = EventNode::KeyType::key_Asterisk;
        break;
    case Qt::Key_ParenLeft:
        eNode.m_keyType = EventNode::KeyType::key_ParenLeft;
        break;
    case Qt::Key_ParenRight:
        eNode.m_keyType = EventNode::KeyType::key_ParenRight;
        break;
    case Qt::Key_Underscore:
        eNode.m_keyType = EventNode::KeyType::key_Underscore;
        break;
    case Qt::Key_Plus:
        eNode.m_keyType = EventNode::KeyType::key_Plus;
        break;
    case Qt::Key_BraceLeft:
        eNode.m_keyType = EventNode::KeyType::key_BraceLeft;
        break;
    case Qt::Key_BraceRight:
        eNode.m_keyType = EventNode::KeyType::key_BraceLeft;
        break;
    case Qt::Key_Bar:
        eNode.m_keyType = EventNode::KeyType::key_Bar;
        break;
    case Qt::Key_Colon:
        eNode.m_keyType = EventNode::KeyType::key_Colon;
        break;
    case Qt::Key_QuoteDbl:
        eNode.m_keyType = EventNode::KeyType::key_QuoteDbl;
        break;
    case Qt::Key_Less:
        eNode.m_keyType = EventNode::KeyType::key_Less;
        break;
    case Qt::Key_Greater:
        eNode.m_keyType = EventNode::KeyType::key_Greater;
        break;
    case Qt::Key_Question:
        eNode.m_keyType = EventNode::KeyType::key_Question;
        break;
    default:
        flag = false;
        break;
    }

    return flag;
}
