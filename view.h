#ifndef VIEW_H
#define VIEW_H

#include <QLabel>
#include "pevent.h"
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
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    //键盘事件
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    bool getKetType(EventNode &eNode, int keyType);
};

#endif // VIEW_H
