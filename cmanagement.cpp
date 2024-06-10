#include "cmanagement.h"
#include "csession.h"
#include "qdebug.h"
#include <iostream>

CManagement::CManagement(QObject *parent)
    : QObject{parent}
//, m_acceptor(m_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), ServerPort))
{
    //在没有异步操作的时候，使io_context等待而不退出run()
    m_work = std::make_shared<boost::asio::io_context::work>(m_ioc);
    //开启io服务
    //start();
    auto test = [this]() {
        try {
            // 开启事件循环
            m_ioc.run();

        } catch (std::exception &e) {
            std::cerr << "Exception" << e.what() << std::endl;
        }
    };

    for (int i = 0; i != THREADS_NUM; ++i) {
        m_threads.create_thread(test);
    }
}

CManagement::~CManagement()
{
    m_threads.join_all();
}

std::shared_ptr<CSession> CManagement::startAccept()
{
    //session只是一种资源可以传给任何一个对象，不用对象树管理
    std::shared_ptr<CSession> session = std::make_shared<CSession>(m_ioc);

    //创建acceptor用于接收连接请求
    m_acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(
        m_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), ServerPort));

    m_acceptor
        ->async_accept(session->socket(),
                       std::bind(&CManagement::handleAccept, this, session, std::placeholders::_1));
    return session;
}

void CManagement::handleAccept(std::shared_ptr<CSession> session,
                               const boost::system::error_code &ec)
{
    if (!ec) {
        emit acceptInfo(true);
        //socket设置
        session->setSocket();
        //开启服务器
        session->serverStart();
        //销毁acceptor
        m_acceptor = nullptr;
    } else {
        emit acceptInfo(false);
        std::cerr << "accept error, error code is " << ec.value() << " error message is "
                  << ec.message() << std::endl;
        //销毁acceptor
        m_acceptor = nullptr;
    }
}

std::shared_ptr<CSession> CManagement::startConnect(QString ip, unsigned short port)
{
    std::shared_ptr<CSession> session = std::make_shared<CSession>(m_ioc,
                                                                               ip,
                                                                               port); //IPv4协议默认
    return session;
}

void CManagement::cancelAccept()
{
    m_acceptor->close();
}

void CManagement::close()
{
    //销毁work
    m_work = nullptr;
}
