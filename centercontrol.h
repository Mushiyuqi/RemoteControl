#ifndef CENTERCONTROL_H
#define CENTERCONTROL_H

#include <QMutex>
#include <QThread>
#include <memory.h>

class CSession;
class CManagement;
class ViewBridge;
class ViewControl;
class PEvent;
class CenterControl : public QThread
{
    Q_OBJECT
    friend ViewControl;

public:
    static CenterControl &instance();
    void show();

signals:
    void connectOver();

private:
    explicit CenterControl(QObject *parent = nullptr);
    ~CenterControl();
    void linkPc(QString &ip, unsigned short port);
    void sharePc();
    void closeSharePc();
    //此函数返回一个 StandardButton 值
    int messageBox(QString title = "", QString text = "");

private:
    ViewBridge *_viewBridge;
    ViewControl *_viewControl;
    CManagement *_cmg;
    std::shared_ptr<CSession> _session;
    std::shared_ptr<ViewControl> _vctrl;

    //是否已经连接
    bool m_connectSuccess = false;

    //线程状态
    QMutex m_mutex;
    enum TStatus { Ok = 0, Err = -1 };
    int m_threadStatus = Err;

protected:
    virtual void run() override;
};

#endif // CENTERCONTROL_H
