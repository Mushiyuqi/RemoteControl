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
    QRect screenGeometry = screen->geometry();
    m_pixmap = pixmap;
    _width = screenGeometry.width() * 0.6;
    _height = screenGeometry.height() * 0.6;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id); //用于将内置的行扩展器参数转换为字符串文字
    m_pixmap.scaled(requestedSize);
    if (size)
        *size = QSize(requestedSize);

    return m_pixmap;
}
