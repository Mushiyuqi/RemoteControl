#include "view.h"
#include <QDebug>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QPixmap>
#include <QScreen>
#include "csession.h"
#include "pevent.h"
#include "viewcontrol.h"
View::View(QWidget *parent)
    : QLabel(parent)
{
    setScaledContents(true);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    setPixmap(pixmap.scaled(pixmap.size(), Qt::IgnoreAspectRatio));
}

void View::setControl(ViewControl *vctrl)
{
    _vctrl = vctrl;
}

void View::mousePressEvent(QMouseEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(), 0, 0);
    if (event->button() == Qt::LeftButton)
        eNode.m_type = EventNode::Type::mouseLeftClick;
    else if (event->button() == Qt::RightButton)
        eNode.m_type = EventNode::Type::mouseRightClick;

    _vctrl->eventAction(eNode);
}

void View::wheelEvent(QWheelEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(0, 0, 0, 0, 0, 0);
    // 获取滚动的角度和方向
    int delta = event->angleDelta().y();

    // 判断滚动的方向
    if (delta > 0) {
        qDebug() << "Mouse wheel scrolled up";
        eNode.m_type = EventNode::Type::mouseScrollUp;
    } else {
        qDebug() << "Mouse wheel scrolled down";
        eNode.m_type = EventNode::Type::mouseScrollDown;
    }
    _vctrl->eventAction(eNode);
}

void View::mouseMoveEvent(QMouseEvent *event) {
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(), 0, 0);
    eNode.m_type = EventNode::Type::mouseMove;

    _vctrl->eventAction(eNode);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(), 0, 0);
    eNode.m_type = EventNode::Type::mouseDouble;
    _vctrl->eventAction(eNode);
    qDebug() << "双击";
}

void View::keyPressEvent(QKeyEvent *event) {
     if (_vctrl->_session->status() == CSession::SocketStatus::Err)
         return;
    EventNode eNode(0, 0, 0, 0, EventNode::Type::keyPress, 0);
    bool flag = false;
    switch (event->key()) {
        case Qt::Key_A:
            eNode.m_keyType = EventNode::KeyType::key_A;
        break;
        case Qt::Key_B:
            eNode.m_keyType = EventNode::KeyType::key_B;
            break;
        default:
            flag = true;
        break;
    }
    if (flag)
        return;

    _vctrl->eventAction(eNode);


    // if (event->key() == Qt::Key_A) {
    //     KeyNode kNode(1); //A
    //     _vctrl->keyPressedAction(kNode);
    // }

    // qDebug() << "keypressEvent";
}
void View::keyReleaseEvent(QKeyEvent *event) {

    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    EventNode eNode(0, 0, 0, 0, EventNode::Type::keyRelease, 0);
    bool flag = false;
    switch (event->key()) {
        case Qt::Key_A:
            eNode.m_keyType = EventNode::KeyType::key_A;
        break;
        case Qt::Key_B:
            eNode.m_keyType = EventNode::KeyType::key_B;
        break;
        default:
            flag = true;
        break;
    }
    if (flag)
        return;

    _vctrl->eventAction(eNode);
}

