#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H
#include <QSize>
#include <memory>
class ViewWindow;
class CenterView;
class CenterControl;
class CSessionThread;
class ViewControl {
    friend CenterControl;
    friend ViewWindow;

public:
    static ViewControl& singleton();
    void resize();

    // 计算窗口适合的比例并返回
    QSize prefferdSize();

    // Todo:将viewwindow里刷新图片大小的方法放到这里
    // 根据label大小比例决定如何缩放图片
    // void updatePixmap(const QPixmap& pixmap);

private:
    ViewControl();
    ~ViewControl() noexcept;

    ViewWindow* _viewWindow;
    CenterView* _centerView;
    std::shared_ptr<CSessionThread> _session;
};

#endif // VIEWCONTROL_H
