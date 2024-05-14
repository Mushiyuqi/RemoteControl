#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>

namespace Ui {
class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewWindow(QWidget *parent = nullptr);
    ~ViewWindow();

private:
    Ui::ViewWindow *ui;
};

#endif // VIEWWINDOW_H
