#pragma once

#include <boost/asio.hpp>
#include <array>
#include <map>
#include <iostream>

class CSession;
class CServer{
public:
    CServer(boost::asio::io_context& ioc, short port);
    void clearSession(std::string uuid);
private:
    void start_accept();
    void handle_accept(std::shared_ptr<CSession> new_session, const boost::system::error_code& ec);

private:
    boost::asio::io_context& _ioc;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::map<std::string, std::shared_ptr<CSession>> _sessions;
};
