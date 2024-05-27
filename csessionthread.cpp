#include "csessionthread.h"
#include <QMutexLocker>
#include "cmanagement.h"
#include "data.h"
#include "qdebug.h"
#include <iostream>

CSessionThread::CSessionThread(boost::asio::io_context &ioc)
    : m_socket(ioc)
    , _ioc(ioc)
{
    m_roleStatus = Role::Server;
    _recvHeadNode = std::make_shared<MsgNode>(HEAD_LENGTH);
    _sendData = std::make_shared<std::array<char, MAX_LENGTH>>();
    m_recvData = std::make_shared<std::array<char, MAX_LENGTH>>();
    _data = new Data(this);
}

CSessionThread::CSessionThread(boost::asio::io_context &ioc, QString ip, unsigned short port)
    : m_socket(ioc)
    , _ioc(ioc)
    , m_ip(ip)
    , m_port(port)

{
    m_roleStatus = Role::Client;
    _recvHeadNode = std::make_shared<MsgNode>(HEAD_LENGTH);
    _sendData = std::make_shared<std::array<char, MAX_LENGTH>>();
    m_recvData = std::make_shared<std::array<char, MAX_LENGTH>>();
    _data = new Data(this);
}

CSessionThread::~CSessionThread() {}

void CSessionThread::serverStart()
{
    if (m_roleStatus != Role::Server)
        return;
    start(); //开启线程发送数据
}

void CSessionThread::clientStart()
{
    if (m_roleStatus != Role::Client)
        return;
    boost::asio::ip::tcp::endpoint remoteEp{boost::asio::ip::address::from_string(
                                                m_ip.toStdString()),
                                            m_port};
    m_socket.async_connect(remoteEp,
                           std::bind(&CSessionThread::handleConnect, this, std::placeholders::_1));
}

boost::asio::ip::tcp::socket &CSessionThread::socket()
{
    return m_socket;
}

void CSessionThread::run()
{
    //全双工的收发

    if (m_roleStatus == Role::Server) {
        // 开启接收数据的监听
        m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                 std::bind(&CSessionThread::handleRead,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));

        // 循环发送数据
        //对socket的状态变量加锁
        m_sSLock.lock();
        while (m_socketStatus == SocketState::Ok) {
            m_sSLock.unlock();
            size_t sendLen = _data->getSendData(_sendData);
            if (sendLen != -1) {
                std::cout << "sendLen is " << sendLen << std::endl;
                send(_sendData->data(), sendLen);
            }
            memset(_sendData->data(), 0, sendLen); //重置m_sendData;
            //等待每2ms发送一次
            msleep(2);
            m_sSLock.lock();
        }
        m_sSLock.unlock();
    } else {
        // 开启接收数据的监听
        m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                 std::bind(&CSessionThread::handleRead,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    }
    quit();
}

void CSessionThread::send(char *msg, std::size_t sendLen)
{
    //对队列的增减，取元素加锁
    QMutexLocker<QMutex> locker(&m_sendLock); // QMutexLocker 构造时加锁，析构时解锁
    bool pending = false;                      // 队列中是否有数据正在发送

    if (_sendQue.size() > 0) {
        pending = true;
    }

    //if (_sendQue.size() < 10)
    std::shared_ptr<MsgNode> sendNode = std::make_shared<MsgNode>(msg, sendLen);
    _sendQue.push(sendNode);

    if (pending)
        return;

    boost::asio::async_write(m_socket,
                             boost::asio::buffer(_sendQue.front()->m_data,
                                                 _sendQue.front()->m_total_len),
                             std::bind(&CSessionThread::handleWrite,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
}

size_t CSessionThread::getRecvData(std::shared_ptr<std::array<char, MAX_LENGTH>> data)
{
    QMutexLocker locker(&m_dataLock);
    memcpy(data->data(), m_recvData->data(), m_dataLen);
    return m_dataLen;
}

void CSessionThread::handleRead(const boost::system::error_code &ec, size_t byt_transferred)
{
    if (!ec) {
        // m_data(原始数据) 已经处理的数据
        size_t copy_len = 0;

        /**
         * 我将 _recvHeadNode 创建在了构造函数中
         * 给 Send 传递  msg 和 len 他就用 MsgNode 的构造函数 自动创建一个完整的包
         */

        while (byt_transferred > 0) {
            if (!_b_head_parse) {
                //消息的头部没有处理
                if (byt_transferred + _recvHeadNode->m_cur_len < HEAD_LENGTH) {
                    // 发送过来的数据和以记录的头部数据之和小于标准头部的长度
                    // 将已发送的数据(发送过来的数据是头部)放入_recvHeadNode中
                    memcpy(_recvHeadNode->m_data + _recvHeadNode->m_cur_len,
                           m_data.data() + copy_len,
                           byt_transferred);
                    _recvHeadNode->m_cur_len += byt_transferred;
                    ::memset(m_data.data(), 0, MAX_LENGTH);
                    m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                             std::bind(&CSessionThread::handleRead,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2));
                    return;
                }
                // 收到的消息比头部多
                size_t head_remain = HEAD_LENGTH
                                     - _recvHeadNode->m_cur_len; // 头部剩余未复制的长度
                memcpy(_recvHeadNode->m_data + _recvHeadNode->m_cur_len,
                       m_data.data() + copy_len,
                       head_remain);
                // 更新以处理的data长度 和 剩余未处理的长度
                copy_len += head_remain;
                byt_transferred -= head_remain;
                // 获取头部数据
                std::size_t data_len = 0;
                memcpy(&data_len, _recvHeadNode->m_data, HEAD_LENGTH);
                //std::cout << "data_len is " << data_len << std::endl;
                // 头部长度非法
                if (data_len > MAX_LENGTH) {
                    std::cerr << "invalid data length is" << data_len << std::endl;
                    return;
                }
                _recvMsgNode = std::make_shared<MsgNode>(data_len);

                // 消息的长度小于头部规定的长度，说明数据未收全，则先将部分消息放到接收节点里
                if (byt_transferred < data_len) {
                    memcpy(_recvMsgNode->m_data + _recvMsgNode->m_cur_len,
                           m_data.data() + copy_len,
                           byt_transferred);
                    _recvMsgNode->m_cur_len += byt_transferred;
                    ::memset(m_data.data(), 0, MAX_LENGTH);

                    // 头部已经处理
                    _b_head_parse = true;
                    m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                             std::bind(&CSessionThread::handleRead,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2));

                    return;
                }

                // 消息的长度大于头部规定的长度，有两条消息，分别处理
                memcpy(_recvMsgNode->m_data + _recvMsgNode->m_cur_len,
                       m_data.data() + copy_len,
                       data_len);
                _recvMsgNode->m_cur_len += data_len;
                copy_len += data_len;
                byt_transferred -= data_len;
                _recvMsgNode->m_data[_recvMsgNode->m_total_len] = '\0';
                // 处理第一条数据

                /**
                 * 接收到完整的数据
                 */
                if (m_roleStatus == Role::Server) {
                    handleReadFromClient();
                } else {
                    handleReadFromServer();
                }

                // 继续轮询剩余未处理的数据
                _b_head_parse = false;
                _recvHeadNode->Clear();
                // 恰好收完
                if (byt_transferred <= 0) {
                    ::memset(m_data.data(), 0, MAX_LENGTH);
                    m_socket.async_read_some(boost::asio::buffer(m_data, MAX_LENGTH),
                                             std::bind(&CSessionThread::handleRead,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2));
                    return;
                }
                continue;

            } else {
                // 消息头部已经解析完了 解析剩余数据

                size_t remain_msg = _recvMsgNode->m_total_len
                                    - _recvMsgNode->m_cur_len; // 还需要的数据长度
                // 收到的数据任然不足剩余未处理的
                if (byt_transferred < remain_msg) {
                    memcpy(_recvMsgNode->m_data + _recvMsgNode->m_cur_len,
                           m_data.data() + copy_len,
                           byt_transferred);
                    _recvMsgNode->m_cur_len += byt_transferred;
                    ::memset(m_data.data(), 0, MAX_LENGTH);
                    m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                             std::bind(&CSessionThread::handleRead,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2));
                    return;
                }

                memcpy(_recvMsgNode->m_data + _recvMsgNode->m_cur_len,
                       m_data.data() + copy_len,
                       remain_msg);
                _recvMsgNode->m_cur_len += remain_msg;
                byt_transferred -= remain_msg;
                copy_len += remain_msg;
                _recvMsgNode->m_data[_recvMsgNode->m_total_len] = '\0';

                /**
                 * 接收到完整的数据
                 */
                if (m_roleStatus == Role::Server) {
                    handleReadFromClient();
                } else {
                    handleReadFromServer();
                }

                // 继续轮询剩余未处理的数据
                _b_head_parse = false;
                _recvHeadNode->Clear();
                // 如果 byt_transferred 恰好等于 remain_msg 调用回调函数
                if (byt_transferred <= 0) {
                    ::memset(m_data.data(), 0, MAX_LENGTH);
                    m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                             std::bind(&CSessionThread::handleRead,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2));
                    return;
                }
                continue;
            }
        }
    } else {
        std::cerr << "handle read failed, error code is " << ec.value() << ", message is "
                  << ec.message() << std::endl;
        {
            QMutexLocker locker(&m_sSLock);
            m_socketStatus = SocketState::Err;
        }
    }
}

void CSessionThread::handleWrite(const boost::system::error_code &ec, size_t byt_transferred)
{
    if (!ec) {
        //对队列的增减，取元素加锁
        QMutexLocker<QMutex> locker(&m_sendLock);
        _sendQue.pop();
        if (_sendQue.empty())
            return;
        auto &msgNode = _sendQue.front();
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(msgNode->m_data, msgNode->m_total_len),
                                 std::bind(&CSessionThread::handleWrite,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    } else {
        std::cerr << "handle write failed, error code is " << ec.value() << ", message is "
                  << ec.message() << std::endl;
        {
            QMutexLocker locker(&m_sSLock);
            m_socketStatus = SocketState::Err;
        }
    }
}

void CSessionThread::handleConnect(const boost::system::error_code &ec)
{
    if (!ec) {
        start(); //开启线程接收数据
    } else {
        std::cerr << "connect failed, error code is " << ec.value() << " error message is "
                  << ec.message() << std::endl;
        {
            QMutexLocker locker(&m_sSLock);
            m_socketStatus = SocketState::Err;
        }
    }
}

void CSessionThread::handleReadFromClient()
{
    std::cout << "I am Server, receive data is " << std::endl;
    QMutexLocker locker(&m_dataLock);
    memcpy(&m_dataLen, _recvHeadNode->m_data, HEAD_LENGTH);
    std::cout << "recvLen is " << m_dataLen << std::endl;
    memcpy(m_recvData->data(), _recvMsgNode->m_data, m_dataLen);
}

void CSessionThread::handleReadFromServer()
{
    std::cout << "I am Client, receive data is " << std::endl;
    QMutexLocker locker(&m_dataLock);
    memcpy(&m_dataLen, _recvHeadNode->m_data, HEAD_LENGTH);
    memcpy(m_recvData->data(), _recvMsgNode->m_data, m_dataLen);
    emit readyForDisplay();
}
