#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>
#include <QSize>

class ViewControl;
namespace Ui {
class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewWindow(ViewControl* ctrl, QWidget* parent = nullptr);

    // 根据label大小比例决定如何缩放图片
    void updatePixmap(const QPixmap& pixmap);

    // 当窗口缩放时，图片跟着缩放
    void resizeEvent(QResizeEvent* event);
    ~ViewWindow();

private:
    Ui::ViewWindow *ui;
    ViewControl* _ctrl;
};

#endif // VIEWWINDOW_H
