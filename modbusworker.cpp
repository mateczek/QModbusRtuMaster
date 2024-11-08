#include "modbusworker.h"
#include<QTcpSocket>
#include<QSerialPort>

modbusWorker::modbusWorker(QObject *parent)
    : QObject{parent}
{

}

void modbusWorker::setComDevice(rtuSettings::_rtuSettings rtu_settings)
{
    if (device!=nullptr){
        device->close();
        device->deleteLater();
        device=nullptr;
    };
    QSerialPort* rs232 = new QSerialPort();
    rs232->setBaudRate(rtu_settings.baudRate);
    rs232->setStopBits(rtu_settings.stopBits);
    rs232->setPortName(rtu_settings.portName);
    rs232->setDataBits(rtu_settings.dataBits);
    if(rs232->open(QIODevice::ReadWrite)){
        emit rs232open();
        device=rs232;//ustaw urządzenie do odbioru ramki modbus na portCom
    }
    else{
        rs232->deleteLater();
    }

}

void modbusWorker::setEthernetDevice(QString ip, int port)
{
    if (device!=nullptr){
        device->close();
        device->deleteLater();
        device=nullptr;
    };
    QTcpSocket* ethernet = new QTcpSocket();
    ethernet->connectToHost(ip,port);
    if(ethernet->waitForConnected(1000)){
        emit ethernetConnect();
        device=ethernet; //ustaw urządzenie do odbioru ramki na ethernet
    }
    else{
        ethernet->deleteLater();
    }
}

void modbusWorker::modbusACK(uint8_t slaveAdr, uint8_t mbFunc, uint16_t regAdr, uint16_t regCount)
{
    QByteArray ba;
    QDataStream ds(&ba,QIODevice::ReadWrite);
    ds<<slaveAdr<<mbFunc<<regAdr<<regCount; //ramka modbus
    calcCRC16(ba); //policz sumę kontrolną
    device->readAll(); //odczytaj śmieci z bufora
    device->write(ba); //wyśli zapytanie modbus
    QElapsedTimer t; //timer do przekroczenia czasu
    t.start();
    while ((device->bytesAvailable()<regCount*2+5)){ //czekaj na pełną ramkę
        device->waitForReadyRead(10);
        if(t.elapsed()>5000){
            device->readAll();
            return;
        }
    }
    QByteArray odp=device->readAll(); //odczytaj ramkę
    if(checkCRC16(odp)){ //sprawdź sumę kontrolną
        union{
            uint64_t zm;
            uint8_t byteTab[8];
        }zmienna;
        zmienna.zm=0;
        int dataSize = odp[2];
        for(int i=0;i<dataSize;i++){
            zmienna.byteTab[i]=odp[3+i]; //przepisanie ramki do zmiennej
        }
        emit readRegister(odp,zmienna.zm,dataSize); //powiadomienie o odczytaniu zmiennej
    }
}

void modbusWorker::calcCRC16(QByteArray &frame)
{
    uint8_t crc_hi = 0xFF, crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i;
    for (int count=0;count<frame.size();count++){
        i = crc_hi ^ static_cast<uint8_t>(frame[count]); /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }
    frame+=static_cast<char>(crc_hi);
    frame+=static_cast<char>(crc_lo);
}

bool modbusWorker::checkCRC16(QByteArray &frame)
{
    uint8_t crc_hi = 0xFF, crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i;
    auto rozmiar = frame.size();
    for (int count=0;count<rozmiar-2;count++){
        i = crc_hi ^ static_cast<uint8_t>(frame[count]); /* calculate the CRC  */
        crc_hi = crc_lo ^ table_crc_hi[i];
        crc_lo = table_crc_lo[i];
    }
    if((frame[rozmiar-2]==static_cast<char>(crc_hi))&&(frame[rozmiar-1]==static_cast<char>(crc_lo))){
        return  true;
    }
    return false;
}

void modbusWorker::tcpError()
{

}
