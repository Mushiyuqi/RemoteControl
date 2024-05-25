#include "viewwindow.h"
#include "ui_viewwindow.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QScreen>

ViewWindow::ViewWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ViewWindow)
{
    ui->setupUi(this);
    setWindowTitle("远程控制系统主界面");
    QScreen* screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    ui->label->setPixmap(pixmap);
    setCentralWidget(ui->label);
}

void ViewWindow::updatePixmap(const QPixmap& pixmap)
{

    QSize labelSize = ui->label->size();
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setPixmap(scaledPixmap);
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

ViewWindow::~ViewWindow()
{
    delete ui;
}
