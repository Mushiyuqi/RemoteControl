#include "widget.h"
#include "./ui_widget.h"
#include "centercontrol.h"
Widget::Widget(CenterControl *cctrl, QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Widget)
    , _cctrl(cctrl)
{
    _ui->setupUi(this);
}

Widget::~Widget()
{
    delete _ui;
}

void Widget::on_shareBtn_clicked()
{
    _cctrl->sharePc();
    _ui->shareBtn->setEnabled(false);
}

void Widget::on_linkBtn_clicked()
{
    _cctrl->linkPc();
    _ui->linkBtn->setEnabled(false);
}
