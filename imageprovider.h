#pragma once

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    void setPixmap(const QPixmap &pixmap)
    {
        if (!pixmap.isNull())
            m_pixmap = pixmap;
    }
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QPixmap m_pixmap;
};
