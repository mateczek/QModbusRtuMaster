#include "modbusThread.h"

modbusThread::modbusThread(QObject *parent)
    : QThread{parent}
{

}

void modbusThread::run() //praca w wątku
{
    modbusWorker mbw; //obiekt silniczek modbusa
    connect(this,SIGNAL(rtuSettings(rtuSettings::_rtuSettings)),&mbw,SLOT(setComDevice(rtuSettings::_rtuSettings)));
    connect(this,SIGNAL(overTcpSettings(QString,int)),&mbw,SLOT(setEthernetDevice(QString,int)));
    connect(this,SIGNAL(modbusACK(uint8_t,uint8_t,uint16_t,uint16_t)),&mbw,SLOT(modbusACK(uint8_t,uint8_t,uint16_t,uint16_t)));
    connect(&mbw,SIGNAL(ethernetConnect()),this,SIGNAL(ethernetConnect()));
    connect(&mbw,SIGNAL(rs232open()),this,SIGNAL(rs232open()));
    connect(&mbw,SIGNAL(readRegister(QByteArray,int64_t,int)),this,SIGNAL(readRegister(QByteArray,int64_t,int)));
    exec();
}



