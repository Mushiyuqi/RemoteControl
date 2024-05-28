#include "cmanagement.h"
#include "csessionthread.h"
#include "qdebug.h"
#include <iostream>

void CManagement::run()
{
    try {
        // 开启事件循环
        m_ioc.run();

    } catch (std::exception &e) {
        std::cerr << "Exception" << e.what() << std::endl;
        quit();
    }
    quit();
}

CManagement::CManagement(QObject *parent)
    : QThread{parent}
//, m_acceptor(m_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), ServerPort))
{
    //在没有异步操作的时候，使io_context等待而不退出run()
    m_work = std::make_shared<boost::asio::io_context::work>(m_ioc);
    //开启io服务
    start();
}

void CManagement::startAccept()
{
    //session只是一种资源可以传给任何一个对象，不用对象树管理
    _session = std::make_shared<CSessionThread>(m_ioc);

    //用于在一台电脑上运行的测试的acceptor的创建位置
    if (m_acceptor == nullptr)
        m_acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(
            m_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), ServerPort));

    m_acceptor->async_accept(_session->socket(),
                             std::bind(&CManagement::handleAccept,
                                       this,
                                       _session,
                                       std::placeholders::_1));

    // m_acceptor
    //     .async_accept(_session->socket(),
    //                   std::bind(&CManagement::handleAccept, this, _session, std::placeholders::_1));
}

void CManagement::handleAccept(std::shared_ptr<CSessionThread> session,
                               const boost::system::error_code &ec)
{
    if (!ec) {
        session->serverStart();
    } else {
        std::cout << "accept error " << std::endl;
    }
}

std::shared_ptr<CSessionThread> CManagement::startConnect(QString ip, unsigned short port)
{
    std::shared_ptr<CSessionThread> session = std::make_shared<CSessionThread>(m_ioc,
                                                                               ip,
                                                                               port); //IPv4协议默认
    return session;
}
