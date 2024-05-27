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

    // Todo:将viewwindow里刷新图片大小的方法放到这里
    // 根据label大小比例决定如何缩放图片
    // void updatePixmap(const QPixmap& pixmap);
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
