#include "data.h"
#include <QImage>
#include <QPixmap>
#include <QtWidgets>
Data::Data(QObject *parent)
    : QObject{parent}
{
}

size_t Data::getSendData(std::shared_ptr<std::array<char, MAX_LENGTH>> sendData)
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    // 将QPixmap直接保存到QByteArray中，使用JPEG格式
    if (!pixmap.save(&buffer, "JPG")) {
        qDebug() << "Failed to save pixmap to JPG";
        byteArray.clear();
        return -1;
    }
    // 检查图片大小是否超出限制
    if (byteArray.size() > MAX_LENGTH) {
        qDebug() << "Image size exceeds MAX_LENGTH";
        byteArray.clear();
        return -1;
    }

    std::copy_n(byteArray.data(), byteArray.size(), sendData->data());
    return byteArray.size();
}

QPixmap Data::transData(std::shared_ptr<std::array<char, MAX_LENGTH>> recvData, size_t recvLen)
{
    QByteArray byteArray(recvData->data(), recvLen);
    QImage image;
    if (!image.loadFromData(byteArray, "JPG")) {
        qDebug() << "Failed to load image from data";
        return QPixmap();
    }
    QPixmap pic = QPixmap::fromImage(image);
    return pic;
}
