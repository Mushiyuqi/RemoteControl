#include "centercontrol.h"
#include <QJsonDocument>
#include <QMessageBox>
#include <QMutexLocker>
#include "cmanagement.h"
#include "csession.h"
#include "pevent.h"
#include "viewcontrol.h"
#include "viewwindow.h"
#include "widget.h"
#include <QDebug>
#include <iostream>

CenterControl::CenterControl(QObject *parent)
    : QThread{parent}
{
    _widget = new Widget(this);   //这里没用对象树
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
                messageBox("SUCCESS", "连接成功.");
            } else {
                m_connectSuccess = false;
                messageBox("ERROR", "连接失败.");
            }
        },
        Qt::QueuedConnection);

    //连接断开
    connect(
        this,
        &CenterControl::connectOver,
        this,
        [this]() {
            messageBox("MESSAGE", "连接断开.");
            _widget->initialBtn();
        },
        Qt::QueuedConnection);
}

CenterControl::~CenterControl()
{
    //关闭session
    if (_session != nullptr) {
        if (m_connectSuccess) {
            _session->socket().close();
        } else {
            _session = nullptr;
            _cmg->cancelAccept();
            _widget->initialBtn();
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

void CenterControl::show()
{
    _widget->show();
}

int CenterControl::messageBox(QString title, QString text)
{
    QMessageBox errMsg;
    errMsg.setWindowTitle(title);
    errMsg.setText(text);
    errMsg.setStandardButtons(QMessageBox::Ok);
    errMsg.setDefaultButton(QMessageBox::Ok);
    return errMsg.exec();
}

void CenterControl::linkPc(QString &ip, unsigned short port)
{
    std::shared_ptr<CSession> session = _cmg->startConnect(ip, port);
    _vctrl = std::make_shared<ViewControl>(session, this);

    //连接失败
    if (session->status() == CSession::SocketStatus::Err) {
        messageBox("ERROR", "Connect Error !\n 请重试.");
        _vctrl = nullptr;
        return;
    }
    //连接成功
    //连接断开
    connect(
        _vctrl.get(),
        &ViewControl::connectOver,
        this,
        [this]() {
            _vctrl = nullptr;
            _widget->setEnabled(true);
            messageBox("MESSAGE", "Connect Error !\n 连接以断开.");
        },
        Qt::QueuedConnection);
    _widget->setEnabled(false);
    _vctrl->_viewWindow->show();
}

void CenterControl::sharePc()
{
    //开始连接监听
    _session = _cmg->startAccept();
}

void CenterControl::closeSharePc()
{
    if (m_connectSuccess) {
        //之所以直接关闭socket是因为run里会阻塞直到有client数据发送过来才有机会获得线程状态的锁，不然就一直阻塞
        _session->socket().close();
    } else {
        _session = nullptr;
        _cmg->cancelAccept();
        _widget->initialBtn();
    }
}

void CenterControl::run()
{
    //创建事件处理器 确保start创建的线程和pEvent在一起
    PEvent pEvent;
    while (1) {
        QMutexLocker<QMutex> lockerThis(&m_mutex);
        //本端关闭
        if (m_threadStatus == TStatus::Err)
            break;

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

        PositionNode pNode = PositionNode::fromJson(jsonObject);
        pEvent.mouseToDo(pNode);

        //做键盘事件处理
        //KeyNode kNode = KeyNode::fromJson(jsonObject);
        //pEvent.KeyTodo(kNode);
        //qDebug() << "执行一次KeyTodo";
    }
    //关闭session
    _session->close();
    _session = nullptr;
    emit connectOver();
    quit();
}
