#ifndef DATA_H
#define DATA_H

#include "msgnode.h"

#include <QObject>
#include <QPixmap>
//压缩比0 - 100  -1（默认值）
#define SHRINKAGE_RATE 6

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = nullptr);

    //将数据转换成字节数据放到sendData里传出去
    size_t getSendData(std::shared_ptr<std::array<char, MAX_LENGTH>> sendData);
    //将接收到的数据recvData转换成可用数据 用return传出
    QPixmap transData(std::shared_ptr<std::array<char, MAX_LENGTH>> recvData, size_t recvLen);

signals:
private:
};

#endif // DATA_H
