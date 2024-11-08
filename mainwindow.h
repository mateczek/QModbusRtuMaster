#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<rtusettings.h>
#include<rtuovertcpsettings.h>
#include<modbusThread.h>
#include<QSettings>
#include<QCloseEvent>
#include<reinterpretervariable.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSettings* settings;
    rtuSettings* rtuSetingsWindow = nullptr;
    rtuOverTcpSettings* tcpSetingnsWindow = nullptr;
    modbusThread modbusControler;
    reinterpreterVariable rw;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionRTU_COM_PORT_triggered();

    void on_actionRTU_OVER_TCP_triggered();

    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
    void restoreSettings();
    void closeEvent(QCloseEvent *event);
    void get16bitValue(uint16_t zm);
    void get32bitValue(uint32_t zm);
    void get64bitValue(uint64_t zm);
    void enable16Groupbox();
    void enable32Groupbox();
    void enable64Groupbox();
};
#endif // MAINWINDOW_H
