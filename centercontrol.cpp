#include "centercontrol.h"
#include <QJsonDocument>
#include <QMessageBox>
#include <QMutexLocker>
#include "cmanagement.h"
#include "csession.h"
#include "pevent.h"
#include "viewbridge.h"
#include "viewcontrol.h"
#include <iostream>

CenterControl::CenterControl(QObject *parent)
    : QThread{parent}
{
    _viewBridge = new ViewBridge(this);
    _cmg = new CManagement(this); //用了对象树

    //接收连接请求成功
    connect(
        _cmg,
        &CManagement::acceptInfo,
        this,
        [this](bool info) {
            if (info) {
                //确保线程在开启前处于关闭状态
                wait();
                //恢复状态
                m_threadStatus = TStatus::Ok;
                m_connectSuccess = true;
                //开启接收事件信息
                start();
                emit _viewBridge->acceptInfo(true);
            } else {
                m_connectSuccess = false;
                emit _viewBridge->acceptInfo(false);
            }
        },
        Qt::QueuedConnection);

    //连接断开
    connect(
        this,
        &CenterControl::connectOver,
        this,
        [this]() {
            m_connectSuccess = false;
            emit _viewBridge->connectSeverOver();
        },
        Qt::QueuedConnection);
}

CenterControl::~CenterControl()
{
    //关闭session
    if (_session != nullptr) {
        if (m_connectSuccess) {
            _session->close();
        } else {
            _session = nullptr;
            _cmg->cancelAccept();
        }
    }
    wait();
    //关闭CManagement的线程池
    _cmg->close();
}

CenterControl &CenterControl::instance()
{
    static CenterControl cctrl;
    return cctrl;
}

ViewBridge *CenterControl::viewBridge()
{
    return _viewBridge;
}

bool CenterControl::linkPc(QString &ip, unsigned short port)
{
    std::shared_ptr<CSession> session = _cmg->startConnect(ip, port);

    //连接失败
    if (session == nullptr) {
        return false;
    }
    //连接成功
    _vctrl = std::make_shared<ViewControl>(session, this, _viewBridge);
    _viewBridge->setViewControl(_vctrl.get());
    //连接断开
    connect(
        _vctrl.get(),
        &ViewControl::connectOver,
        this,
        [this]() {
            _viewBridge->setViewControl(nullptr);
            _vctrl = nullptr;
            emit _viewBridge->connectClientOver();
        },
        Qt::QueuedConnection);
    return true;
}

void CenterControl::sharePc()
{
    //开始连接监听
    _session = _cmg->startAccept();
}

void CenterControl::closeSharePc()
{
    if (m_connectSuccess) {
        _session->close();
    } else {
        _session = nullptr;
        _cmg->cancelAccept();
    }
    m_connectSuccess = false;
}

void CenterControl::run()
{
    //创建事件处理器 确保start创建的线程和pEvent在一起
    PEvent pEvent;
    while (1) {
        QMutexLocker<QMutex> lockerSession(&(_session->m_recvDataLock));
        _session->m_waiter.wait(&(_session->m_recvDataLock));

        //判断是否任然连接中
        if (_session->status() == CSession::SocketStatus::Err) {
            m_threadStatus = TStatus::Err;
            break;
        }

        QByteArray byteArray(_session->m_recvData->data(), _session->m_recvDataLen);
        QString jsonString = QString::fromUtf8(byteArray);

        // 将JSON字符串解析为QJsonObject
        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
        QJsonObject jsonObject = jsonDocument.object();

         EventNode eNode = EventNode::fromJson(jsonObject);
        pEvent.eventToDo(eNode);
    }
    //关闭session
    _session->close();
    _session = nullptr;
    emit connectOver();
    quit();
}
