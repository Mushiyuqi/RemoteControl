#include "data.h"

Data::Data(QObject *parent)
    : QObject{parent}
{}

bool Data::getSendData(std::shared_ptr<std::array<char, MAX_LENGTH>> sendData)
{
    int i;
    for (i = 0; i != MAX_LENGTH / 2 - 1024; ++i) {
        (*sendData)[i] = '0';
    }
    (*sendData)[i] = '\0';
    return true;
}

bool Data::transData(std::shared_ptr<std::array<char, MAX_LENGTH>> recvData) {}
