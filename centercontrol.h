#ifndef CENTERCONTROL_H
#define CENTERCONTROL_H

#include <QThread>
#include <memory.h>

class CSessionThread;
class CManagement;
class Widget;
class ViewControl;
class PEvent;
class CenterControl : public QThread
{
    Q_OBJECT
    friend Widget;
    friend ViewControl;

public:
    static CenterControl &instance();
    void show();

private:
    explicit CenterControl(QObject *parent = nullptr);
    ~CenterControl();
    void linkPc();
    void sharePc();

private:
    Widget *_widget;
    ViewControl* _viewControl;
    CManagement *_cmg;
    PEvent *_event;
    std::shared_ptr<CSessionThread> _session;

    //线程状态
    enum TStatus { Ok = 0, Err = -1 };
    int m_threadStatus = Ok;

    //没想好vctrl怎么管理先放到智能指针里
    std::shared_ptr<ViewControl> _vctrl;

protected:
    virtual void run() override;
};

#endif // CENTERCONTROL_H
