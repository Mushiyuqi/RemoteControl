#include "viewcontrol.h"
#include "centercontrol.h"
#include "csessionthread.h"
#include "data.h"
#include "view.h"
#include "viewwindow.h"
ViewControl::ViewControl(std::shared_ptr<CSessionThread> session, CenterControl *cctrl)
    : _session(session)
    , _cctrl(cctrl)
{
    m_dataBuffer = std::make_shared<std::array<char, MAX_LENGTH>>(); //创建缓冲区
    _session->clientStart(); //开启客户端
    _data = std::make_shared<Data>(); //创建数据处理组
    _viewWindow = new ViewWindow(this);
    _view = _viewWindow->centralWidget();
    _view->setSession(session);
    _viewWindow->setWindowTitle("远程控制系统主界面");
    _viewWindow->show();
    start(); //开启刷新线程
}

ViewControl::~ViewControl() noexcept{}

void ViewControl::updatePixmap()
{
    size_t dataLen = _data->getSendData(m_dataBuffer);
    // 转换为图片
    // 更新图片
    _view->setPixmap(_data->transData(m_dataBuffer, dataLen));
}

void ViewControl::run()
{
    while (m_threadStatus == TStatus::Ok) {
        updatePixmap();
    }
    quit();
}
