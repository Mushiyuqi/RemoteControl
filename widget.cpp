#include "widget.h"
#include "./ui_widget.h"
#include "centercontrol.h"
Widget::Widget(CenterControl *cctrl, QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Widget)
    , _cctrl(cctrl)
{
    _ui->setupUi(this);
    _ui->shareBtn->setEnabled(true);
    _ui->closeShareBtn->setEnabled(false);
    _ui->linkBtn->setEnabled(true);
}

Widget::~Widget()
{
    delete _ui;
}

void Widget::on_shareBtn_clicked()
{
    _ui->shareBtn->setEnabled(false);
    _ui->closeShareBtn->setEnabled(true);
    _ui->linkBtn->setEnabled(false);
    _cctrl->sharePc();
}

void Widget::on_closeShareBtn_clicked()
{
    _cctrl->closeSharePc();
}

void Widget::on_linkBtn_clicked()
{
    QString ip = _ui->ipEdit->text();
    unsigned short port = _ui->portEdit->text().toUShort();
    _cctrl->linkPc(ip, port);
}

void Widget::initialBtn()
{
    _ui->shareBtn->setEnabled(true);
    _ui->closeShareBtn->setEnabled(false);
    _ui->linkBtn->setEnabled(true);
}
