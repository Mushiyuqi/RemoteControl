#include "viewcontrol.h"
#include <QJsonDocument>
#include <QMutex>
#include <QMutexLocker>
#include "centercontrol.h"
#include "csession.h"
#include "data.h"
#include "viewbridge.h"
ViewControl::ViewControl(std::shared_ptr<CSession> session,
                         CenterControl *cctrl,
                         ViewBridge *viewBridge)
    : _session(session)
    , _cctrl(cctrl)
{
    m_dataBuffer = std::make_shared<std::array<char, MAX_LENGTH>>(); //创建缓冲区
    _data = std::make_shared<Data>(); //创建数据处理组
    _viewBridge = viewBridge;         //viewwindow部分与前端交互

    start(); //开启刷新线程
}

ViewControl::~ViewControl() noexcept
{
    //关闭session防止直接在主程序关闭
    _session->close();

    //等待线程结束
    wait();
}

void ViewControl::closeConnect()
{
    QMutexLocker<QMutex> locker(&m_mutex);
    m_threadStatus = TStatus::Err;
}

void ViewControl::run()
{
    while (true) {
        msleep(2); //防止饥饿
        QMutexLocker<QMutex> lockerThis(&m_mutex);
        //本端关闭
        if (m_threadStatus == TStatus::Err)
            break;

        QMutexLocker<QMutex> lockerSession(&(_session->m_recvDataLock));
        //判断对端是否关闭
        if (_session->status() == CSession::SocketStatus::Err) {
            m_threadStatus = TStatus::Err;
            break;
        }

        _session->m_waiter.wait(&(_session->m_recvDataLock));
        //判断对端是否关闭
        if (_session->status() == CSession::SocketStatus::Err) {
            m_threadStatus = TStatus::Err;
            break;
        }

        memcpy(m_dataBuffer->data(), _session->m_recvData->data(), _session->m_recvDataLen);
        m_dataLen = _session->m_recvDataLen;
        _viewBridge->updatePixmap(std::move(_data->transData(m_dataBuffer, m_dataLen)));
    }
    //关闭session
    _session->close();
    emit connectOver();
    quit();
}

void ViewControl::mouseMoveAcction(PositionNode p)
{
    //转换为json字符串
    QJsonDocument jsonDocument(p.toJson());
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
    this->_session->send(jsonString.toStdString().data(), jsonString.length());
}
void ViewControl::mouseClickedAcction(PositionNode p)
{
    //转换为json字符串
    QJsonDocument jsonDocument(p.toJson());
    QString jsonString = jsonDocument.toJson(QJsonDocument::Compact);
    this->_session->send(jsonString.toStdString().data(), jsonString.length());
}
