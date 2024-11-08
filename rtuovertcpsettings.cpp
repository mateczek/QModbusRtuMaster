#include "rtuovertcpsettings.h"
#include "ui_rtuovertcpsettings.h"

rtuOverTcpSettings::rtuOverTcpSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::rtuOverTcpSettings)
{
    ui->setupUi(this);
}

rtuOverTcpSettings::~rtuOverTcpSettings()
{
    delete ui;
}

std::tuple<QString, int> rtuOverTcpSettings::getBoxSettings()
{
    return std::make_tuple(ui->lineEdit->text(), ui->lineEdit_2->text().toInt());
}

void rtuOverTcpSettings::setBoxSettings(const QString &ip, int port)
{
    ui->lineEdit->setText(ip);
    ui->lineEdit_2->setText(QString::number(port));
}

void rtuOverTcpSettings::on_buttonBox_accepted()
{
    ip=ui->lineEdit->text();
    port=ui->lineEdit_2->text().toInt();
    emit setModbusTCP(ip,port);
}

