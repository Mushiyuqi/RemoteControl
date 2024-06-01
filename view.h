#ifndef VIEW_H
#define VIEW_H

#include <QLabel>
#include <memory>
class Data;
class CSessionThread;
class ViewControl;
class View : public QLabel
{
    Q_OBJECT
    friend ViewControl;

public:
    View(QWidget *parent = nullptr);

private:
    void setSession(std::shared_ptr<CSessionThread> session);
    std::shared_ptr<CSessionThread> _session;

protected:
    //鼠标事件
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    //键盘事件
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // VIEW_H
