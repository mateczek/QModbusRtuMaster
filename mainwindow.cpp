#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDataStream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->CB_Adress_offset->addItem("brak",0);
    ui->CB_Adress_offset->addItem("minus",-1);
    ui->CB_Adress_offset->addItem("plus",1);

    ui->CB_MB_function->addItem("ReadHoldingRegister",3);
    ui->CB_MB_function->addItem("ReadInputRegister",4);

    ui->CB_Reg_count->addItem("16Bit 1-rejestr",1);
    ui->CB_Reg_count->addItem("32Bit 2-rejestry",2);
    ui->CB_Reg_count->addItem("64Bit 4-rejestry",4);
    rtuSetingsWindow=new rtuSettings(this);
    tcpSetingnsWindow=new rtuOverTcpSettings(this);
    ui->radioCom->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->radioCom->setFocusPolicy(Qt::NoFocus);
    ui->radioEth->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->radioEth->setFocusPolicy(Qt::NoFocus);
    restoreSettings();
    //connect(rtuSetingsWindow,SIGNAL(setPortCom(rtuSettings::_rtuSettings&)),&modbusControler,SIGNAL(rtuSettings(rtuSettings::_rtuSettings&)));
    connect(rtuSetingsWindow,rtuSettings::setPortCom,[this](rtuSettings::_rtuSettings par){
         emit modbusControler.rtuSettings(par);
    });
    connect(tcpSetingnsWindow,SIGNAL(setModbusTCP(QString,int)),&modbusControler,SIGNAL(overTcpSettings(QString,int)));


    connect(&modbusControler,&modbusThread::readRegister,[this](QByteArray ba,int64_t zm,int count){
        ui->lineEdit->setText(ba.toHex(' '));
        switch (count) {
        case 2:
            get16bitValue(zm);
            break;
        case 4:
            get32bitValue(zm);
            break;
        case 8:
            get64bitValue(zm);
            break;
        default:
            break;
        }
    });
    connect(&modbusControler,&modbusThread::rs232open,[this](){
        ui->radioCom->setChecked(true);
        ui->radioEth->setChecked(false);
    });
    connect(&modbusControler,&modbusThread::ethernetConnect,[this](){
        ui->radioCom->setChecked(false);
        ui->radioEth->setChecked(true);
    });
    modbusControler.start();
}

MainWindow::~MainWindow()
{
    modbusControler.terminate();
    modbusControler.wait();
    delete ui;
}




void MainWindow::on_actionRTU_COM_PORT_triggered()
{
    rtuSetingsWindow->show();
}


void MainWindow::on_actionRTU_OVER_TCP_triggered()
{
    tcpSetingnsWindow->show();
}




void MainWindow::on_toolButton_clicked()
{
    uint8_t sa =ui->LE_Slave_adress->text().toInt(); //pobranie adresu modbusa
    uint8_t mbFunc=ui->CB_MB_function->currentData().value<uint8_t>();//numer funkcji modbus
    int8_t ofsetAdr=ui->CB_Adress_offset->currentData().value<uint8_t>(); //offset adresu +1 -1 0;
    uint16_t adressReg=ui->LE_Mb_adress->text().toInt()+ofsetAdr; //adres rejestru
    uint16_t regCount=ui->CB_Reg_count->currentData().value<int16_t>(); //liczba rejestrów do pobrania
    emit modbusControler.modbusACK(sa,mbFunc,adressReg,regCount);

}

void MainWindow::restoreSettings()
{
    settings=new QSettings("MarcoCorporation","Q_ModbusRTU_Master",this);
    settings->beginGroup("RS232Window");
        int portNameBoxIndex=settings->value("portNameBoxIndex",0).toInt();
        int speedBoxIndex=settings->value("speedBoxIndex",3).toInt();
        int dataBitsBoxIndex=settings->value("dataBitsBoxIndex",0).toInt();
        int parityBoxIndex=settings->value("parityBoxIndex",0).toInt();
        rtuSetingsWindow->setBoxIndex(portNameBoxIndex,speedBoxIndex,dataBitsBoxIndex,parityBoxIndex);
    settings->endGroup();
    settings->beginGroup("tcpipWindow");
        QString  ipRtuDevice=settings->value("ipRtuDevice","0.0.0.0").toString();
        int portRtuDevice=settings->value("portRtuDevice",502).toInt();
        tcpSetingnsWindow->setBoxSettings(ipRtuDevice,portRtuDevice);
    settings->endGroup();
    settings->beginGroup("modbusSettings");
        ui->LE_Mb_adress->setText(settings->value("modbusAdress",5000).toString());
        ui->LE_Slave_adress->setText(settings->value("slaveAdress",1).toString());
    settings->endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    auto[portNameBoxIndex,speedBoxIndex,dataBitsBoxIndex,parityBoxIndex]=rtuSetingsWindow->getSetiingsBox();
        settings->beginGroup("RS232Window");
        settings->setValue("portNameBoxIndex",portNameBoxIndex);
        settings->setValue("speedBoxIndex",speedBoxIndex);
        settings->setValue("dataBitsBoxIndex",dataBitsBoxIndex);
        settings->setValue("parityBoxIndex",parityBoxIndex);
    settings->endGroup();
    auto[ip,port]=tcpSetingnsWindow->getBoxSettings();
    settings->beginGroup("tcpipWindow");
        settings->setValue("ipRtuDevice",ip);
        settings->setValue("portRtuDevice",port);
    settings->endGroup();
    settings->beginGroup("modbusSettings");
        settings->setValue("modbusAdress",ui->LE_Mb_adress->text());
        settings->value("slaveAdress",ui->LE_Slave_adress->text());
    settings->endGroup();
    event->accept();


}

void MainWindow::get16bitValue(uint16_t zm)
{
    auto[intAB,intBA,uintAB,uintBA]=rw.get16bitValue(zm);
    ui->LE_INT_AB->setText(QString::number(intAB));
    ui->LE_INT_BA->setText(QString::number(intBA));
    ui->LE_UINT_AB->setText(QString::number(uintAB));
    ui->LE_UINT_BA->setText(QString::number(uintBA));
    enable16Groupbox();

}

void MainWindow::get32bitValue(uint32_t zm)
{
    auto[intABCD,uintABCD,floatABCD,intCDAB,uintCDAB,floatCDAB,intDCBA,uintDCBA,floatDCBA,intBADC,uintBADC,floatBADC]=rw.get32bitValue(zm);
    ui->LE_INT32_ABCD->setText(QString::number(intABCD));
    ui->LE_UINT32_ABCD->setText(QString::number(uintABCD));
    ui->LE_FLOAT_ABCD->setText(QString::number(floatABCD));

    ui->LE_INT32_CDAB->setText(QString::number(intCDAB));
    ui->LE_UINT32_CDAB->setText(QString::number(uintCDAB));
    ui->LE_FLOAT_CDAB->setText(QString::number(floatCDAB));

    ui->LE_INT32_DCBA->setText(QString::number(intDCBA));
    ui->LE_UINT32_DCBA->setText(QString::number(uintDCBA));
    ui->LE_FLOAT_DCBA->setText(QString::number(floatDCBA));

    ui->LE_INT32_BADC->setText(QString::number(intBADC));
    ui->LE_UINT32_BADC->setText(QString::number(uintBADC));
    ui->LE_FLOAT_BADC->setText(QString::number(floatBADC));
    enable32Groupbox();
}

void MainWindow::get64bitValue(uint64_t zm)
{
    auto[intABCDEFGH,uintABCDEFGH,doubleABCDEFGH,intHGFEDCBA,uintHGFEDCBA,doubleHGFEDCBA]=rw.get64bitValue(zm);
    ui->LE_INT64_ABCD->setText(QString::number(intABCDEFGH));
    ui->LE_UINT64_ABCD->setText(QString::number(uintABCDEFGH));
    ui->LE_DOUBLE_ABCD->setText(QString::number(doubleABCDEFGH));

    ui->LE_INT64_DCBA->setText(QString::number(intHGFEDCBA));
    ui->LE_UINT64_DCBA->setText(QString::number(uintHGFEDCBA));
    ui->LE_DOUBLE_DCBA->setText(QString::number(doubleHGFEDCBA));
    enable64Groupbox();

}

void MainWindow::enable16Groupbox()
{
    ui->groupBox_3->setEnabled(true);
    ui->groupBox_4->setEnabled(true);
    ui->groupBox_5->setEnabled(true);
    ui->groupBox_6->setEnabled(true);
    ui->groupBox_7->setEnabled(false);
    ui->groupBox_8->setEnabled(false);
    ui->groupBox_9->setEnabled(false);
    ui->groupBox_10->setEnabled(false);
    ui->groupBox_11->setEnabled(false);
    ui->groupBox_12->setEnabled(false);
    ui->groupBox_13->setEnabled(false);
    ui->groupBox_14->setEnabled(false);
    ui->groupBox_15->setEnabled(false);
    ui->groupBox_16->setEnabled(false);
    ui->groupBox_17->setEnabled(false);
    ui->groupBox_18->setEnabled(false);
    ui->groupBox_19->setEnabled(false);
    ui->groupBox_20->setEnabled(false);
    ui->groupBox_21->setEnabled(false);
    ui->groupBox_22->setEnabled(false);
    ui->groupBox_23->setEnabled(false);
    ui->groupBox_24->setEnabled(false);


}

void MainWindow::enable32Groupbox()
{
    ui->groupBox_3->setEnabled(false);
    ui->groupBox_4->setEnabled(false);
    ui->groupBox_5->setEnabled(false);
    ui->groupBox_6->setEnabled(false);
    ui->groupBox_7->setEnabled(true);
    ui->groupBox_8->setEnabled(true);
    ui->groupBox_9->setEnabled(true);
    ui->groupBox_10->setEnabled(true);
    ui->groupBox_11->setEnabled(true);
    ui->groupBox_12->setEnabled(true);
    ui->groupBox_13->setEnabled(true);
    ui->groupBox_14->setEnabled(true);
    ui->groupBox_15->setEnabled(false);
    ui->groupBox_16->setEnabled(false);
    ui->groupBox_17->setEnabled(false);
    ui->groupBox_18->setEnabled(false);
    ui->groupBox_19->setEnabled(false);
    ui->groupBox_20->setEnabled(false);
    ui->groupBox_21->setEnabled(true);
    ui->groupBox_22->setEnabled(true);
    ui->groupBox_23->setEnabled(true);
    ui->groupBox_24->setEnabled(true);
}

void MainWindow::enable64Groupbox()
{
    ui->groupBox_3->setEnabled(false);
    ui->groupBox_4->setEnabled(false);
    ui->groupBox_5->setEnabled(false);
    ui->groupBox_6->setEnabled(false);
    ui->groupBox_7->setEnabled(false);
    ui->groupBox_8->setEnabled(false);
    ui->groupBox_9->setEnabled(false);
    ui->groupBox_10->setEnabled(false);
    ui->groupBox_11->setEnabled(false);
    ui->groupBox_12->setEnabled(false);
    ui->groupBox_13->setEnabled(false);
    ui->groupBox_14->setEnabled(false);
    ui->groupBox_15->setEnabled(true);
    ui->groupBox_16->setEnabled(true);
    ui->groupBox_17->setEnabled(true);
    ui->groupBox_18->setEnabled(true);
    ui->groupBox_19->setEnabled(true);
    ui->groupBox_20->setEnabled(true);
    ui->groupBox_21->setEnabled(false);
    ui->groupBox_22->setEnabled(false);
    ui->groupBox_23->setEnabled(false);
    ui->groupBox_24->setEnabled(false);
}





