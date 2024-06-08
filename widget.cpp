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
    QString ip = _ui->ipEdit->text();
    unsigned short port = _ui->portEdit->text().toUShort();
    _cctrl->linkPc(ip, port);
}

void Widget::on_closeShareBtn_clicked() {}

void Widget::initial()
{
    _ui->shareBtn->setEnabled(true);
    _ui->closeShareBtn->setEnabled(true);
}
