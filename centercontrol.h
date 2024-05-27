#ifndef CENTERCONTROL_H
#define CENTERCONTROL_H

#include <QObject>
#include <memory.h>

class CManagement;
class Widget;
class ViewControl;
class CenterControl : public QObject
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

    //没想好vctrl怎么管理先放到智能指针里
    std::shared_ptr<ViewControl> _vctrl;
signals:
};

#endif // CENTERCONTROL_H
