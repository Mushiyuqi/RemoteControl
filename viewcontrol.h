#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H

class ViewWindow;
class CenterView;
class CenterControl;

class ViewControl {
    friend CenterControl;
    friend ViewWindow;

public:
    static ViewControl& singleton();
    void resize();

private:
    ViewControl();
    ~ViewControl() noexcept;
    // Todo:将viewwindow里刷新图片大小的方法放到这里

    ViewWindow* _viewWindow;
    CenterView* _centerView;
};

#endif // VIEWCONTROL_H
