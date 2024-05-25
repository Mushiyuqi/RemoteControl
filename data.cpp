#include "data.h"
#include <QPixmap>
#include <QtWidgets>

Data::Data(QObject *parent)
    : QObject{parent}
{
    //  QScreen* screen = QGuiApplication::primaryScreen();
}

bool Data::getSendData(std::shared_ptr<std::array<char, MAX_LENGTH>> sendData)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    QImage image = pixmap.toImage();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    // 检查图片大小是否超出限制
    if (byteArray.size() > MAX_LENGTH) {
        qDebug() << "Failed to trans screen to data";
        byteArray.clear();
        return false;
    }
    std::copy(byteArray.constBegin(), byteArray.constEnd(), sendData->begin());
    return true;
}

QPixmap Data::transData(std::shared_ptr<std::array<char, MAX_LENGTH>> recvData)
{
    QByteArray byteArray(recvData->data(), recvData->size());
    QImage image;
    if (!image.loadFromData(byteArray, "PNG")) {
        qDebug() << "Failed to load image from data";
        return QPixmap();
    }
    QPixmap pic = QPixmap::fromImage(image);

    return pic;
}
