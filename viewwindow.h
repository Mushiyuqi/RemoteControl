#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>
#include <QSize>
namespace Ui {
class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewWindow(QWidget *parent = nullptr);

    // 根据label大小比例决定如何缩放图片
    void updatePixmap(const QPixmap& pixmap);

    // 当窗口缩放时，图片跟着缩放
    void resizeEvent(QResizeEvent* event);
    ~ViewWindow();

private:
    Ui::ViewWindow *ui;
};

#endif // VIEWWINDOW_H
