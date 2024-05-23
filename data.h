#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "msgnode.h"

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = nullptr);

    //将数据转换成字节数据放到sendData里传出去
    bool getSendData(std::shared_ptr<std::array<char, MAX_LENGTH>> sendData);
    //将接收到的数据recvData转换成可用数据 用return传出
    bool transData(std::shared_ptr<std::array<char, MAX_LENGTH>> recvData);

signals:
};

#endif // DATA_H
