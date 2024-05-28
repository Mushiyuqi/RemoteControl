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
    setFixedSize(prefferdSize());
}

ViewWindow::~ViewWindow()
{
    delete _ui;
}

void ViewWindow::updatePixmap(const QPixmap& pixmap)
{
    // QSize labelSize = _ui->label->size();
    // QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // _ui->label->setAlignment(Qt::AlignCenter);
    // _ui->label->setPixmap(scaledPixmap);
}

void ViewWindow::resizeEvent(QResizeEvent* event)
{
    // 当窗口大小改变时，更新label的pixmap
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    // 更新pixmap以适应新的窗口大小
    updatePixmap(pixmap);
    QMainWindow::resizeEvent(event);
}

QSize ViewWindow::prefferdSize()
{
    QSize prefferdsize(_ui->centralwidget->width() * 0.4, _ui->centralwidget->height() * 0.4 + 25);
    return prefferdsize;
}

View *ViewWindow::centralWidget()
{
    return _ui->centralwidget;
}
