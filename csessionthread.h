#ifndef CSESSIONTHREAD_H
#define CSESSIONTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include "msgnode.h"
#include <boost/asio.hpp>
#include <queue>
#define SEND_QUEUE_LEN 30 //长度必须大于等于2

class Data;
class CManagement;
class CSessionThread : public QThread
{
    Q_OBJECT
public:
    explicit CSessionThread(boost::asio::io_context &ioc);
    explicit CSessionThread(boost::asio::io_context &ioc, QString ip, unsigned short port);
    ~CSessionThread();

    void serverStart();
    void clientStart();

    //发送数据
    void send(char *msg, std::size_t max_length);

    //socket数据
    enum SocketStatus { Ok = 0, Err = -1 }; //连接的状态
    boost::asio::ip::tcp::socket &socket(); //获取socket
    int status();                           //获取session的状态

    //提供给外面的完整数据
    QMutex m_recvDataLock;
    QWaitCondition m_waiter;
    std::shared_ptr<std::array<char, MAX_LENGTH>> m_recvData;
    size_t m_recvDataLen;

protected:
    virtual void run() override;

private:
    // 读回调
    void handleRead(const boost::system::error_code &ec, size_t byt_transferred);
    // 写回调
    void handleWrite(const boost::system::error_code &ec, size_t byt_transferred);
    // 连接回调
    void handleConnect(const boost::system::error_code &ec);

    // 作为接收了完整数据之后的操作
    void handleData();

private:
    boost::asio::io_context &_ioc;
    Data *_data;

    //是否作为服务器方
    //是否作为客户端方
    enum Role { Server = true, Client = false };
    bool m_roleStatus = Server;

    //socket数据结构
    QMutex m_sSLock;                       //管理socket状态的改变
    int m_socketStatus = Ok;               //为Err时就不能发送数据了
    boost::asio::ip::tcp::socket m_socket; // 自己管理的socket

    //连接数据结构
    QString m_ip;          //对端的ip
    unsigned short m_port; //对端的port
    //boost::system::error_code m_ec{boost::asio::error::host_not_found}; //连接的错误码

    //发送数据结构
    QMutex m_sendLock;                                       //管理发送队列的锁
    int _currentSendingQueLen;
    int _sendFront; //队列的头部
    int _sendBack; //队列的尾部
    std::vector<std::shared_ptr<MsgNode>> _sendQue;       //发送队列

    std::shared_ptr<std::array<char, MAX_LENGTH>> _sendData; //发送的原始数据

    //接收数据结构
    QMutex m_displayNodeLock;
    std::array<char, MAX_LENGTH> m_data;      // 接收的原始数据
    bool _b_head_parse;                       // 消息头部（数据长度）是否处理
    std::shared_ptr<MsgNode> _recvMsgNode; // 收到的消息体信息 里面的数据可能不是完整的
    std::shared_ptr<MsgNode> _recvHeadNode;   // 收到的头部结构

    // todo 接收的鼠标事件信息
    int x, y; // 收到的鼠标位置信息
};

#endif // CSESSIONTHREAD_H
