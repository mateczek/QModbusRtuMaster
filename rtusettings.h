#ifndef RTUSETTINGS_H
#define RTUSETTINGS_H

#include <QDialog>
#include<QtSerialPort>
#include<tuple>
namespace Ui {
class rtuSettings;
}

class rtuSettings : public QDialog
{
    Q_OBJECT

public:
    struct _rtuSettings{
        QSerialPort::BaudRate baudRate;
        QString portName;
        QSerialPort::Parity parity;
        QSerialPort::DataBits dataBits;
        QSerialPort::StopBits stopBits;
    };
    void setBoxIndex(int portBoxIndex,int speedBoxIndex,int dataBitBoxIndex,int parityBitBoxIndex);
    std::tuple<int,int,int,int> getSetiingsBox();


    explicit rtuSettings(QWidget *parent = nullptr);
    ~rtuSettings();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::rtuSettings *ui;
    _rtuSettings rtu_Settings;

signals:
    void setPortCom (rtuSettings::_rtuSettings&);
};

#endif // RTUSETTINGS_H
