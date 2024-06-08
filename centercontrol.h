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

private slots:
    void on_viewcontrol_over(bool info);

private:
    explicit CenterControl(QObject *parent = nullptr);
    ~CenterControl();
    void linkPc(QString &ip, unsigned short port);
    void sharePc();
    //此函数返回一个 StandardButton 值
    int messageBox(QString title = "", QString text = "");

private:
    Widget *_widget;
    ViewControl* _viewControl;
    CManagement *_cmg;
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
