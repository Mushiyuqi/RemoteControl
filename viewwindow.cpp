#include "viewwindow.h"
#include "ui_viewwindow.h"
#include "viewcontrol.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QScreen>

ViewWindow::ViewWindow(ViewControl *ctrl, QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::ViewWindow)
    , _vctrl(ctrl)
{
    _ui->setupUi(this);
    // this->resize(prefferdSize());
}

ViewWindow::~ViewWindow()
{
    delete _ui;
}

View *ViewWindow::centralWidget()
{
    return _ui->centralwidget;
}

void ViewWindow::closeEvent(QCloseEvent *event)
{
    _vctrl->closeConnect();
    QMainWindow::closeEvent(event);
}
