#include "view.h"
#include <QGuiApplication>
#include <QJsonDocument>
#include <QMouseEvent>
#include <QPixmap>
#include <QScreen>
#include "csessionthread.h"
#include "pevent.h"
#include <iostream>
View::View(QWidget *parent)
    : QLabel(parent)
{
    setScaledContents(true);
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    setPixmap(pixmap.scaled(pixmap.size(), Qt::IgnoreAspectRatio));
}

void View::setSession(std::shared_ptr<CSessionThread> session)
{
    _session = session;
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    // std::cout << "label is clicked. x:" << event->pos().x() << " y:" << event->pos().y()
    //           << std::endl;
    if (_session->status() == CSessionThread::SocketStatus::Err)
        return;
    PositionNode pNode(event->pos().x(),
                       event->pos().y(),
                       (double) event->pos().x() / QLabel::width(),
                       (double) event->pos().y() / QLabel::height(),
                       PositionNode::Type::mouseRelease);
    //转换为json字符串
    QJsonDocument jsonDocument(pNode.toJson());
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
    _session->send(jsonString.toStdString().data(), jsonString.length());
}

void View::mouseMoveEvent(QMouseEvent *event) {}

void View::keyPressEvent(QKeyEvent *event) {}

void View::mouseDoubleClickEvent(QMouseEvent *event) {}

void View::keyReleaseEvent(QKeyEvent *event) {}
