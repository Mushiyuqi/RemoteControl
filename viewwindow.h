#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>
#include <QSize>

class ViewControl;
class View;
namespace Ui {
class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT
    friend ViewControl;

public:
    explicit ViewWindow(ViewControl* ctrl, QWidget* parent = nullptr);
    ~ViewWindow();

    // 根据label大小比例决定如何缩放图片
    void updatePixmap(const QPixmap& pixmap);
    // 计算窗口适合的比例并返回
    QSize prefferdSize();

    // 当窗口缩放时，图片跟着缩放
    void resizeEvent(QResizeEvent* event);

private:
    View *centralWidget();

private:
    Ui::ViewWindow *_ui;
    ViewControl *_vctrl;
};

#endif // VIEWWINDOW_H
