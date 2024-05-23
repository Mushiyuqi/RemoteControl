#include "widget.h"
#include "./ui_widget.h"
#include "centercontrol.h"

Widget::Widget(CenterControl *cctrl, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , _cctrl(cctrl)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_shareBtn_clicked()
{
    _cctrl->sharePc();
}

void Widget::on_linkBtn_clicked()
{
    _cctrl->linkPc();
}
