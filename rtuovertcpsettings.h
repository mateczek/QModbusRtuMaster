#ifndef RTUOVERTCPSETTINGS_H
#define RTUOVERTCPSETTINGS_H
//klasa okienka ustawień ip
#include <QDialog>
#include<tuple>

namespace Ui {
class rtuOverTcpSettings;
}

class rtuOverTcpSettings : public QDialog
{
    Q_OBJECT
    QString ip;
    int port;

public:
    explicit rtuOverTcpSettings(QWidget *parent = nullptr);
    ~rtuOverTcpSettings();
    std::tuple<QString,int>getBoxSettings();
    void setBoxSettings(const QString&ip, int port);
private:
    Ui::rtuOverTcpSettings *ui;
signals:
    void setModbusTCP(QString ip,int port);
private slots:
    void on_buttonBox_accepted();
};

#endif // RTUOVERTCPSETTINGS_H
