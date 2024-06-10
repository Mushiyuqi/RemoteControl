#include "widget.h"
#include <QMessageBox>
#include <QRegularExpression>
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
    QRegularExpression ipv4Regex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2["
                                 "0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpression portRegex(
        "^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");

    QRegularExpressionMatch ipv4Match = ipv4Regex.match(_ui->ipEdit->text());
    QRegularExpressionMatch portMatch = portRegex.match(_ui->portEdit->text());

    if (!ipv4Match.hasMatch() || !portMatch.hasMatch()) {
        QMessageBox::warning(this, "", "ip or port error");
        return;
    }
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
