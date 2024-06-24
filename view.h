#ifndef VIEW_H
#define VIEW_H

#include <QLabel>
class Data;
class CSession;
class ViewControl;
class View : public QLabel
{
    Q_OBJECT
    friend ViewControl;

public:
    View(QWidget *parent = nullptr);

private:
    void setControl(ViewControl *vctrl);
    ViewControl *_vctrl;

protected:
    //鼠标事件
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;

    //键盘事件
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // VIEW_H
