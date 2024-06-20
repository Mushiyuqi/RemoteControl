#pragma once

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    void setPixmap(const QPixmap &pixmap);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

    int m_width;
    int m_height;

private:
    QPixmap m_pixmap;
};
