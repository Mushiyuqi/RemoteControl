#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H
#include <QSize>
#include <QThread>
#include <memory>
class ViewWindow;
class View;
class CenterControl;
class CSessionThread;
class Data;
class ViewControl : public QThread
{
public:
    ViewControl(std::shared_ptr<CSessionThread> session, CenterControl *cctrl);
    ~ViewControl() noexcept;

    void resize();

    // 接收图片用来改变view显示的图片
    void updatePixmap();

private:
    //顶级控件
    CenterControl *_cctrl;

    //显示组件
    ViewWindow *_viewWindow;
    View *_view;

    //工具组件
    std::shared_ptr<CSessionThread> _session; //用于获取数据
    std::shared_ptr<Data> _data;              //用于做数据处理

protected:
    virtual void run() override;
};

#endif // VIEWCONTROL_H
