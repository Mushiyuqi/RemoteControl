#ifndef CENTERCONTROL_H
#define CENTERCONTROL_H

#include <QObject>

class CManagement;
class Widget;
// class ViewWindow;
class ViewControl;
class CenterControl : public QObject
{
    Q_OBJECT
    friend Widget;
    friend ViewControl;

public:
    static CenterControl &instance();
    void show();

private:
    explicit CenterControl(QObject *parent = nullptr);
    ~CenterControl();
    void linkPc();
    void sharePc();

private:
    Widget *_widget;
    // ViewWindow *_viewWindow;
    ViewControl* _viewControl;
    CManagement *_cmg;

signals:
};

#endif // CENTERCONTROL_H
