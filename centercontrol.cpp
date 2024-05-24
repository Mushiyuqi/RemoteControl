#include "centercontrol.h"
#include "cmanagement.h"
#include "csessionthread.h"
#include "widget.h"

CenterControl::CenterControl(QObject *parent)
    : QObject{parent}
{
    _widget = new Widget(this);
    _cmg = new CManagement(this);
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
    //std::shared_ptr<CSessionThread> session = _cmg->startConnect("10.252.178.72", 10086);
}

void CenterControl::sharePc()
{
    _cmg->startAccept();
}
