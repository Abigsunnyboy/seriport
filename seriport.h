#ifndef SERIPORT_H
#define SERIPORT_H

#include <QMainWindow>

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class Seriport;
}

class Seriport : public QMainWindow
{
    Q_OBJECT

public:
    explicit Seriport(QWidget *parent = 0);
    ~Seriport();
    void init();
    void initUi();

    void createActions();
    QStringList getSerialNameList();

private slots:
    void on_openCloseSerialPushButton_clicked();

    void on_ClearDataPushbutton_clicked();

    void readSetting();

    void on_SenddatapushButton_clicked();

    void on_QuitpushButton_clicked();

    void sendData();

    void on_checkBoxTimer_toggled(bool checked);

    void on_hexSentcheckBox_toggled(bool checked);

    void on_pushButtonSaveData_clicked();

private:
    Ui::Seriport *ui;
    QTimer *autoSendTimer;
    QSerialPort *Myserial;
};

#endif // SERIPORT_H
