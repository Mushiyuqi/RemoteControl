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
    _session->clientStart(); //开启客户端
    _data = std::make_shared<Data>();
    _viewWindow = new ViewWindow(this);
    _view = _viewWindow->centralWidget();
    _view->setSession(session);
    _viewWindow->setWindowTitle("远程控制系统主界面");
    _viewWindow->show();
}

ViewControl::~ViewControl() noexcept{}

void ViewControl::updatePixmap()
{ // 获取data
    std::shared_ptr<std::array<char, MAX_LENGTH>> data;
    _session->getRecvData(data);
    // 转换为图片
    QPixmap pixmap = _data->transData(data);
    // 更新图片
    _view->setPixmap(pixmap);
}

void ViewControl::run() {}
