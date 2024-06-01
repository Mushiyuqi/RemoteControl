#ifndef CMANAGEMENT_H
#define CMANAGEMENT_H
#include <QThread>
#include <boost/asio.hpp>
#define ServerPort 10086

class CSessionThread;
class CManagement : public QThread
{
    Q_OBJECT
public:
    explicit CManagement(QObject *parent = nullptr);
    std::shared_ptr<CSessionThread> startAccept();
    std::shared_ptr<CSessionThread> startConnect(QString ip, unsigned short port);

private:
    void handleAccept(std::shared_ptr<CSessionThread> new_session,
                      const boost::system::error_code &ec);

private:
    std::shared_ptr<boost::asio::io_context::work> m_work; //控制io_context
    boost::asio::io_context m_ioc;                         //io上下文
    //boost::asio::ip::tcp::acceptor m_acceptor;             //监听连接请求
    std::shared_ptr<CSessionThread> _session;              //用于分享屏幕的session

    //因为想在一个电脑上运行两个网络程序就不能在初始化的时候分配端口
    std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor; //用于测试的acceptor

protected:
    virtual void run() override;
};

#endif // CMANAGEMENT_H
