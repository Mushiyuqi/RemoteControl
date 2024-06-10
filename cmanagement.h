#ifndef CMANAGEMENT_H
#define CMANAGEMENT_H
#include <QThread>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#define ServerPort 10086
#define THREADS_NUM 10

class CSession;
class CManagement : public QObject
{
    Q_OBJECT
public:
    explicit CManagement(QObject *parent = nullptr);
    ~CManagement();
    std::shared_ptr<CSession> startAccept();
    std::shared_ptr<CSession> startConnect(QString ip, unsigned short port);
    void cancelAccept();
    void close(); //程序结束关闭忙等待的io_context

signals:
    void acceptInfo(bool info);

private:
    void handleAccept(std::shared_ptr<CSession> new_session,
                      const boost::system::error_code &ec);

private:
    std::shared_ptr<boost::asio::io_context::work> m_work; //控制io_context
    boost::asio::io_context m_ioc;                         //io上下文
    //boost::asio::ip::tcp::acceptor m_acceptor;             //监听连接请求

    //因为想在一个电脑上运行两个网络程序就不能在初始化的时候分配端口
    std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor; //用于测试的acceptor

    //线程池
    boost::thread_group m_threads;
};

#endif // CMANAGEMENT_H
