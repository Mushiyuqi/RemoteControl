#include "viewwindow.h"
#include "ui_viewwindow.h"

ViewWindow::ViewWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ViewWindow)
{
    ui->setupUi(this);
}

ViewWindow::~ViewWindow()
{
    delete ui;
}
