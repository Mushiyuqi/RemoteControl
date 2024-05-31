#include "centercontrol.h"
#include "cmanagement.h"
#include "csessionthread.h"
#include "pevent.h"
#include "viewcontrol.h"
#include "widget.h"

CenterControl::CenterControl(QObject *parent)
    : QThread{parent}
{
    _widget = new Widget(this);
    _cmg = new CManagement(this);
    _event = new PEvent(this);
}

CenterControl::~CenterControl()
{
}

CenterControl &CenterControl::instance()
{
    static CenterControl cctrl;
    return cctrl;
}

void CenterControl::show()
{
    _widget->show();
}

void CenterControl::linkPc()
{
    std::shared_ptr<CSessionThread> session = _cmg->startConnect("127.0.0.1", 10086);
    _vctrl = std::make_shared<ViewControl>(session, this);
    _widget->hide();
}

void CenterControl::sharePc()
{
    _cmg->startAccept();
}

void CenterControl::run() {}
