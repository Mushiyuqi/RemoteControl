#include "imageprovider.h"
#include <QApplication>
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>
ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    QScreen *screen = QApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    pixmap.fill(Qt::black);
    m_pixmap = pixmap;
    m_width = pixmap.width();
    m_height = pixmap.height();
}

void ImageProvider::setPixmap(const QPixmap &pixmap)
{
    m_width = pixmap.width();
    m_height = pixmap.height();
    m_pixmap = pixmap;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (size)
        *size = QSize(m_pixmap.width(), m_pixmap.height());

    return m_pixmap;
}
