#include "centercontrol.h"
#include <QJsonDocument>
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
    std::shared_ptr<CSessionThread> session = _cmg->startConnect("10.252.106.97", 10086);
    _vctrl = std::make_shared<ViewControl>(session, this);
    _widget->hide();
}

void CenterControl::sharePc()
{
    _session = _cmg->startAccept();
    start();
}

void CenterControl::run()
{
    //创建事件处理器 确保start创建的线程和pEvent在一起
    PEvent pEvent;
    while (m_threadStatus == TStatus::Ok) {
        QMutexLocker<QMutex> locker(&(_session->m_recvDataLock));
        _session->m_waiter.wait(&(_session->m_recvDataLock));
        QByteArray byteArray(_session->m_recvData->data(), _session->m_recvDataLen);
        QString jsonString = QString::fromUtf8(byteArray);
        //std::cout << jsonString.toStdString() << std::endl;

        // 将JSON字符串解析为QJsonObject
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonObject jsonObject = jsonDocument.object();

        // 从QJsonObject构造PositionNode实例
        PositionNode pNode = PositionNode::fromJson(jsonObject);

        //做事件处理
        pEvent.toDo(pNode);
    }
    quit();
}
