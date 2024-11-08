#include "rtusettings.h"
#include "ui_rtusettings.h"
#include <qDebug>
void rtuSettings::setBoxIndex(int portBoxIndex, int speedBoxIndex, int dataBitBoxIndex, int parityBitBoxIndex)
{
    ui->comboBox->setCurrentIndex(portBoxIndex);
    ui->comboBox_2->setCurrentIndex(speedBoxIndex);
    ui->comboBox_3->setCurrentIndex(dataBitBoxIndex);
    ui->comboBox_4->setCurrentIndex(parityBitBoxIndex);
}

std::tuple<int, int, int, int> rtuSettings::getSetiingsBox()
{
    return std::make_tuple(ui->comboBox->currentIndex(),ui->comboBox_2->currentIndex(),ui->comboBox_3->currentIndex(),ui->comboBox_4->currentIndex());
}

rtuSettings::rtuSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::rtuSettings)
{

    ui->setupUi(this);
    const auto listport = QSerialPortInfo::availablePorts();
    for (const auto & el:listport){
        ui->comboBox->addItem(el.portName(),el.portName());
        //qDebug()<<el;
    }
    ui->comboBox_2->addItem("2400",QSerialPort::Baud2400);
    ui->comboBox_2->addItem("4800",QSerialPort::Baud4800);
    ui->comboBox_2->addItem("9600",QSerialPort::Baud9600);
    ui->comboBox_2->addItem("19200",QSerialPort::Baud19200);
    ui->comboBox_2->addItem("38400",QSerialPort::Baud38400);
    ui->comboBox_2->addItem("57600",QSerialPort::Baud57600);
    ui->comboBox_2->addItem("115200",QSerialPort::Baud115200);
    ui->comboBox_3->addItem("7 Bitów Danych",QSerialPort::Data7);
    ui->comboBox_3->addItem("8 Bitów Danych",QSerialPort::Data8);
    ui->comboBox_4->addItem("NoParity",QSerialPort::NoParity);
    ui->comboBox_4->addItem("EvenParity",QSerialPort::EvenParity);
    ui->comboBox_4->addItem("OddParity",QSerialPort::OddParity);
    ui->comboBox_4->addItem("MarkParity",QSerialPort::MarkParity);
    ui->comboBox_4->addItem("SpaceParity",QSerialPort::SpaceParity);

}

rtuSettings::~rtuSettings()
{
    delete ui;
}

void rtuSettings::on_buttonBox_accepted()
{
    rtu_Settings.baudRate= ui->comboBox_2->currentData().value<QSerialPort::BaudRate>();
    rtu_Settings.portName=ui->comboBox->currentData().value<QString>();
    rtu_Settings.parity= ui->comboBox_4->currentData().value<QSerialPort::Parity>();
    rtu_Settings.dataBits=ui->comboBox_3->currentData().value<QSerialPort::DataBits>();
    rtu_Settings.stopBits=QSerialPort::OneStop;
    emit setPortCom(rtu_Settings);
}

