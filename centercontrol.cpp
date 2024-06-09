#include "centercontrol.h"
#include <QJsonDocument>
#include <QMessageBox>
#include "cmanagement.h"
#include "csession.h"
#include "pevent.h"
#include "viewcontrol.h"
#include "viewwindow.h"
#include "widget.h"
#include <iostream>

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

void CenterControl::on_viewcontrol_over(bool info)
{
    //删除connect数据
    _vctrl = nullptr;
    _widget->show();
    if (!info)
        messageBox("ERROR", "Connect Error !\n 对方已经关闭连接.");
    else
        messageBox("MESSAGE", "Connect Error !\n 连接以断开.");
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
    connect(_vctrl.get(), &ViewControl::connectOver, this, &CenterControl::on_viewcontrol_over);
    _widget->hide();
    _vctrl->_viewWindow->show();
}

void CenterControl::sharePc()
{
    _session = _cmg->startAccept();
    connect(_cmg, &CManagement::acceptInfo, this, [this](bool info) {
        if (info) {
            //开启接收事件信息
            start();
            messageBox("SUCCESS", "连接成功.");
        } else
            messageBox("ERROR", "连接失败.");
    });
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
