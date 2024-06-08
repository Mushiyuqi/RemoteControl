#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class CenterControl;
class Widget : public QWidget
{
    Q_OBJECT
    friend CenterControl;

public:
    Widget(CenterControl *cctrl, QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_shareBtn_clicked();

    void on_linkBtn_clicked();

    void on_closeShareBtn_clicked();

private:
    void initial();

private:
    Ui::Widget *_ui;
    CenterControl *_cctrl;
};
#endif // WIDGET_H
