#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H
#include <QMutex>
#include <QSize>
#include <QThread>
#include "msgnode.h"
#include <memory>
#include <pevent.h>
class ViewBridge;
class CenterControl;
class CSession;
class Data;
class ViewControl : public QThread
{
    Q_OBJECT
    friend CenterControl;
    friend ViewBridge;

public:
    ViewControl(std::shared_ptr<CSession> session, CenterControl *cctrl, ViewBridge *viewBridge);
    ~ViewControl() noexcept;
    //本端关闭
    void closeConnect();

    enum TStatus { Ok = 0, Err = -1 };
signals:
    void connectOver();

private:
    //顶级控件
    CenterControl *_cctrl;

    //桥梁组件
    ViewBridge *_viewBridge;

    //缓存数据
    std::shared_ptr<std::array<char, MAX_LENGTH>> m_dataBuffer;
    size_t m_dataLen;

    //线程的状态
    QMutex m_mutex;
    int m_threadStatus = TStatus::Ok;

    //工具组件
    std::shared_ptr<CSession> _session; //用于获取数据
    std::shared_ptr<Data> _data;        //用于做数据处理

    void eventAction(EventNode eNode);

protected:
    //接收图片用来改变view
    virtual void run() override;
};

#endif // VIEWCONTROL_H
