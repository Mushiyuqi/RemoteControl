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

private:
    View *centralWidget();

private:
    Ui::ViewWindow *_ui;
    ViewControl *_vctrl;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // VIEWWINDOW_H
