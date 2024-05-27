#include "centerview.h"
#include "csessionthread.h"
#include "data.h"
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>
#include <memory>
CenterView::CenterView(QWidget* parent)
    : QLabel(parent)
{
    setScaledContents(true);

    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    setPixmap(pixmap);
    _width = pixmap.width();
    _height = pixmap.height();
}

int CenterView::width()
{
    return _width;
}

int CenterView::height()
{
    return _height;
}

void CenterView::onReadyForDisplay()
{
}
