#include "viewcontrol.h"
#include <QMutex>
#include <QMutexLocker>
#include "centercontrol.h"
#include "csessionthread.h"
#include "data.h"
#include "view.h"
#include "viewwindow.h"
#include <iostream>
ViewControl::ViewControl(std::shared_ptr<CSessionThread> session, CenterControl *cctrl)
    : _session(session)
    , _cctrl(cctrl)
{
    m_dataBuffer = std::make_shared<std::array<char, MAX_LENGTH>>(); //创建缓冲区
    _session->clientStart(); //开启客户端
    _data = std::make_shared<Data>(); //创建数据处理组
    _viewWindow = new ViewWindow(this);
    _view = _viewWindow->centralWidget();
    _view->setControl(this);
    _viewWindow->setWindowTitle("远程控制系统主界面");
    if (_session->status() == CSessionThread::SocketStatus::Ok)
        start(); //开启刷新线程
}

ViewControl::~ViewControl() noexcept
{
    //关闭窗口
    _viewWindow->close();

    //终止线程
    m_threadStatus = TStatus::Err;
    wait();
}

void ViewControl::closeConnect()
{
    emit connectOver(true);
}

void ViewControl::run()
{
    while (m_threadStatus == TStatus::Ok) {
        QMutexLocker<QMutex> locker(&(_session->m_recvDataLock));
        _session->m_waiter.wait(&(_session->m_recvDataLock));
        //判断对端是否关闭
        if (_session->status() == CSessionThread::SocketStatus::Err) {
            emit connectOver(false);
            quit();
        }
        memcpy(m_dataBuffer->data(), _session->m_recvData->data(), _session->m_recvDataLen);
        m_dataLen = _session->m_recvDataLen;
        _view->setPixmap(_data->transData(m_dataBuffer, m_dataLen));
    }
    quit();
}
