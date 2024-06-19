#include "imageprovider.h"
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>
ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    QPixmap pixmap(100, 100);
    pixmap.fill(Qt::black);
    m_pixmap = pixmap;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id); //用于将内置的行扩展器参数转换为字符串文字
    m_pixmap.scaled(requestedSize);
    if (size)
        *size = QSize(requestedSize);

    return m_pixmap;
}
