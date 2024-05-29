#include "view.h"
#include <QGuiApplication>
#include <QMouseEvent>
#include <QPixmap>
#include <QScreen>
#include "csessionthread.h"
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
    std::cout << "label is clicked. x:" << event->pos().x() << " y:" << event->pos().y()
              << std::endl;
    if (_session->status() == CSessionThread::SocketStatus::Err)
        return;

    QString str("x:1 y:2");
    _session->send(str.toStdString().data(), str.length());
}

void View::mouseMoveEvent(QMouseEvent *event) {}

void View::keyPressEvent(QKeyEvent *event) {}
