#pragma once

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include "msgnode.h"
#include <boost/asio.hpp>
#define SEND_QUEUE_LEN 5                //长度必须大于等于2
#define SOCKET_BUF_SIZE 1024 * 1024 * 2 //socket内部的buf

class Data;
class CManagement;
class CSession : public std::enable_shared_from_this<CSession>
{
public:
    explicit CSession(boost::asio::io_context &ioc);
    explicit CSession(boost::asio::io_context &ioc, QString ip, unsigned short port);
    ~CSession();

    //socket数据
    enum SocketStatus { Ok = 0, Err = 1 };  //连接的状态
    boost::asio::ip::tcp::socket &socket(); //获取socket
    bool setSocket();                       //对socket做额外的初始化,必须在open后使用
    int status();                           //获取session的状态

    void serverStart();                           //开启服务器发送和监听
    bool clientStart();                           //开启客户端监听
    void send(char *msg, std::size_t max_length); //发送数据 线程安全
    void close();                                 //主动关闭socket

public:
    //提供给外面的完整数据
    QMutex m_recvDataLock;
    QWaitCondition m_waiter;
    std::shared_ptr<std::array<char, MAX_LENGTH>> m_recvData;
    size_t m_recvDataLen;

private:
    // 读回调
    void handleRead(const boost::system::error_code &ec,
                    size_t byt_transferred,
                    std::shared_ptr<CSession> &_selfShared);
    // 写回调
    void handleWrite(const boost::system::error_code &ec,
                     size_t byt_transferred,
                     std::shared_ptr<CSession> &_selfShared);

    // 作为接收了完整数据之后的操作
    void handleData();

private:
    boost::asio::io_context &_ioc;             //处理异步事件
    std::shared_ptr<Data> _data;               //处理数据
    std::shared_ptr<std::thread> m_sendThread; //服务器循环发送线程

    //连接数据结构
    QString m_ip;          //对端的ip
    unsigned short m_port; //对端的port

    //是否作为服务器方
    //是否作为客户端方
    enum Role { Server = true, Client = false };
    bool m_roleStatus = Server;

    //socket数据结构
    QMutex m_socketSatusLock;              //管理socket状态的改变
    int m_socketStatus = Err;              //为Err时就不能发送数据了
    boost::asio::ip::tcp::socket m_socket; // 自己管理的socket

    //发送数据结构
    //发送数据队列不由io线程管理需要自己管理线程互斥
    QMutex m_sendLock;                                       //管理发送队列的锁
    int _currentSendingQueLen;
    int _sendFront; //队列的头部
    int _sendBack; //队列的尾部
    std::vector<std::shared_ptr<MsgNode>> _sendQue;       //发送队列
    std::shared_ptr<std::array<char, MAX_LENGTH>> _sendData; //发送的原始数据

    //接收数据结构
    //由于是在io线程执行不用担心线程问题
    std::array<char, MAX_LENGTH> m_data;      // 接收的原始数据
    bool _b_head_parse;                       // 消息头部（数据长度）是否处理
    std::shared_ptr<MsgNode> _recvMsgNode; // 收到的消息体信息 里面的数据可能不是完整的
    std::shared_ptr<MsgNode> _recvHeadNode;   // 收到的头部结构
};
