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

    //接收缓冲区初始化
    _recvMsgNode = std::make_shared<MsgNode>(MAX_LENGTH);
    _recvHeadNode = std::make_shared<MsgNode>(HEAD_LENGTH);

    //发送队列的初始化
    _currentSendingQueLen = 0;
    _sendData = std::make_shared<std::array<char, MAX_LENGTH>>();
    for(int i = 0; i != SEND_QUEUE_LEN; ++i)
    {
        _sendQue.push_back(std::make_shared<MsgNode>(HEAD_LENGTH + MAX_LENGTH));
    }
    _sendFront = 0;
    _sendBack = 0;

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

    //接收缓冲区初始化
    _recvMsgNode = std::make_shared<MsgNode>(MAX_LENGTH);
    _recvHeadNode = std::make_shared<MsgNode>(HEAD_LENGTH);

    //发送队列的初始化
    _currentSendingQueLen = 0;
    _sendData = std::make_shared<std::array<char, MAX_LENGTH>>();
    for(int i = 0; i != SEND_QUEUE_LEN; ++i)
    {
        _sendQue.push_back(std::make_shared<MsgNode>(HEAD_LENGTH + MAX_LENGTH));
    }
    _sendFront = 0;
    _sendBack = 0;

    m_recvData = std::make_shared<std::array<char, MAX_LENGTH>>();
    _data = new Data(this);
}

CSessionThread::~CSessionThread()
{
    m_socket.close();
    wait();
}

void CSessionThread::serverStart()
{
    if (m_roleStatus != Role::Server)
        return;
    //调用本函数代表在accpet时连接成功
    m_socketStatus = Ok;
    start(); //开启线程发送数据
}

bool CSessionThread::clientStart()
{
    if (m_roleStatus != Role::Client)
        return false;
    boost::asio::ip::tcp::endpoint remoteEp{boost::asio::ip::address::from_string(
                                                m_ip.toStdString()),
                                            m_port};
    boost::system::error_code ec{boost::asio::error::host_not_found};
    m_socket.connect(remoteEp, ec);
    if (!ec) {
        {
            setSocket();
            QMutexLocker locker(&m_sSLock);
            m_socketStatus = SocketStatus::Ok;
        }
        std::cout << "Connect success " << std::endl;
        // 开启接收数据的监听
        m_socket.async_read_some(boost::asio::buffer(m_data.data(), MAX_LENGTH),
                                 std::bind(&CSessionThread::handleRead,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
        return true;
    } else {
        std::cerr << "connect failed, error code is " << ec.value() << " error message is "
                  << ec.message() << std::endl;
        {
            QMutexLocker locker(&m_sSLock);
            m_socketStatus = SocketStatus::Err;
        }
        return false;
    }
}

bool CSessionThread::setSocket()
{
    //只能在open后设置所以要单独拿出来
    //socket设置
    boost::system::error_code ec;
    m_socket.set_option(boost::asio::ip::tcp::no_delay(true), ec); //设置立即发送
    m_socket.set_option(boost::asio::socket_base::receive_buffer_size(SOCKET_BUF_SIZE),
                        ec); //接收缓冲区
    m_socket.set_option(boost::asio::socket_base::send_buffer_size(SOCKET_BUF_SIZE),
                        ec); //发送缓冲区
    if (ec) {
        std::cerr << "socket setting failed, error code is " << ec.value() << ", message is "
                  << ec.message() << std::endl;
        return false;
    }
    return true;
}

boost::asio::ip::tcp::socket &CSessionThread::socket()
{
    return m_socket;
}

int CSessionThread::status()
{
    QMutexLocker locker(&m_sSLock);
    return m_socketStatus;
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
        while (m_socketStatus == SocketStatus::Ok) {
            m_sSLock.unlock();
            size_t sendLen = _data->getSendData(_sendData);
            if (sendLen != -1) {
                send(_sendData->data(), sendLen);
            }
            memset(_sendData->data(), 0, sendLen); //重置m_sendData;
            m_sSLock.lock();
        }
        m_sSLock.unlock();
    }
    quit();
}

void CSessionThread::send(char *msg, std::size_t sendLen)
{
    //对队列的增减，取元素加锁
    QMutexLocker<QMutex> locker(&m_sendLock); // QMutexLocker 构造时加锁，析构时解锁
    bool pending = false;                      // 队列中是否有数据正在发送

    //有数据正在发送
    _currentSendingQueLen = (_sendBack - _sendFront + SEND_QUEUE_LEN) % SEND_QUEUE_LEN;
    if (_currentSendingQueLen > 0) {
        pending = true;
    }

    //允许将数据放入发送队列
    if (_currentSendingQueLen < SEND_QUEUE_LEN - 1)
    {
        //直接做覆盖 不需要clear
        _sendQue[_sendBack]->m_total_len = HEAD_LENGTH + sendLen;
        _sendQue[_sendBack]->m_cur_len = 0;
        memcpy(_sendQue[_sendBack]->m_data, &sendLen, HEAD_LENGTH);// 将消息长度写入m_data
        memcpy(_sendQue[_sendBack]->m_data+ HEAD_LENGTH, msg, sendLen);// 将消息内容写入m_data
        _sendBack = (_sendBack + 1) % SEND_QUEUE_LEN;
        std::cout << "sendLen is " << sendLen << std::endl;
    }

    if (pending)
        return;

    boost::asio::async_write(m_socket,
                             boost::asio::buffer(_sendQue[_sendFront]->m_data,
                                                 _sendQue[_sendFront]->m_total_len),
                             std::bind(&CSessionThread::handleWrite,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
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

                //重置接收缓冲区
                _recvMsgNode->m_total_len = data_len;
                _recvMsgNode->m_cur_len = 0;

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
                handleData();

                // 继续轮询剩余未处理的数据
                _b_head_parse = false;
                _recvHeadNode->clear();
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
                handleData();

                // 继续轮询剩余未处理的数据
                _b_head_parse = false;
                _recvHeadNode->clear();
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
            m_socketStatus = SocketStatus::Err;
            m_waiter.wakeAll(); //唤醒等待数据的view
        }
    }
}

void CSessionThread::handleWrite(const boost::system::error_code &ec, size_t byt_transferred)
{
    if (!ec) {
        //对队列的增减，取元素加锁
        std::cout << "handleWrite byt_transferred is " << byt_transferred << std::endl;
        QMutexLocker<QMutex> locker(&m_sendLock);
        _sendFront = (_sendFront + 1) % SEND_QUEUE_LEN;
        _currentSendingQueLen = (_sendBack - _sendFront + SEND_QUEUE_LEN) % SEND_QUEUE_LEN;
        if (_currentSendingQueLen <= 0)
            return;
        boost::asio::async_write(m_socket,
                                 boost::asio::buffer(_sendQue[_sendFront]->m_data, _sendQue[_sendFront]->m_total_len),
                                 std::bind(&CSessionThread::handleWrite,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    } else {
        std::cerr << "handle write failed, error code is " << ec.value() << ", message is "
                  << ec.message() << std::endl;
        {
            QMutexLocker locker(&m_sSLock);
            m_socketStatus = SocketStatus::Err;
        }
    }
}

void CSessionThread::handleData()
{
    QMutexLocker locker(&m_recvDataLock);
    memcpy(&m_recvDataLen, _recvHeadNode->m_data, HEAD_LENGTH);
    std::cout << "receive dataLen is " << m_recvDataLen << std::endl;
    memcpy(m_recvData->data(), _recvMsgNode->m_data, m_recvDataLen);
    m_waiter.wakeAll();
}
