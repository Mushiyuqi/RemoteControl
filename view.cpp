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
    PositionNode pNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height());
    if (event->button() == Qt::LeftButton)
        pNode.m_type = PositionNode::Type::mouseLeftPress;
    else if (event->button() == Qt::RightButton)
        pNode.m_type = PositionNode::Type::mouseRightPress;

    _vctrl->mouseAction(pNode);
}
void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    PositionNode pNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height());
    if (event->button() == Qt::LeftButton)
        pNode.m_type = PositionNode::Type::mouseLeftRelease;
    else if (event->button() == Qt::RightButton)
        pNode.m_type = PositionNode::Type::mouseRightRelease;

    _vctrl->mouseAction(pNode);
}

void View::mouseMoveEvent(QMouseEvent *event) {
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    PositionNode pNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height());
    pNode.m_type = PositionNode::Type::mouseMove;

    _vctrl->mouseAction(pNode);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (_vctrl->_session->status() == CSession::SocketStatus::Err)
        return;
    PositionNode pNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height());
    pNode.m_type = PositionNode::Type::mouseDouble;
    _vctrl->mouseAction(pNode);
}

void View::keyPressEvent(QKeyEvent *event) {
    // if (_vctrl->_session->status() == CSession::SocketStatus::Err)
    //     return;
    // if (event->key() == Qt::Key_A) {
    //     KeyNode kNode(1); //A
    //     _vctrl->keyPressedAction(kNode);
    // }

    // qDebug() << "keypressEvent";
}

