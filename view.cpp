#include "view.h"
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
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    setPixmap(pixmap.scaled(pixmap.size(), Qt::IgnoreAspectRatio));
}

void View::setControl(ViewControl *vctrl)
{
    _vctrl = vctrl;
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
    //转换为json字符串
    QJsonDocument jsonDocument(pNode.toJson());
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
    _vctrl->_session->send(jsonString.toStdString().data(), jsonString.length());
}

void View::mouseMoveEvent(QMouseEvent *event) {}

void View::mouseDoubleClickEvent(QMouseEvent *event) {}

void View::keyReleaseEvent(QKeyEvent *event) {}
