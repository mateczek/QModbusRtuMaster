#ifndef MODBUSTHREAD_H
#define MODBUSTHREAD_H

#include <QThread>
#include<modbusworker.h>
class modbusThread : public QThread
{
    Q_OBJECT

    void run();
public:
    explicit modbusThread(QObject *parent = nullptr);

public slots:



signals:
    void rtuSettings(rtuSettings::_rtuSettings rtu_settings);
    void overTcpSettings(QString ip,int port);
    void readRegister(QByteArray frame,int64_t reg,int byteCount);
    void modbusACK(uint8_t slaveArd,uint8_t modbusFunc,uint16_t modbusAdres,uint16_t regCount);
    void rs232open();
    void ethernetConnect();
};

#endif // MODBUSTHREAD_H
