#include "seriport.h"
#include "ui_seriport.h"
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QDebug>
#include <QMessageBox>
#include <QFile>
Seriport::Seriport(QWidget *parent) :
    QMainWindow(parent),

    ui(new Ui::Seriport)
{
    ui->setupUi(this);
    init();
    initUi();

}

Seriport::~Seriport()
{
    delete ui;
}

void Seriport::init()
{

}
void Seriport::initUi()
{
setWindowTitle(QString::fromLocal8Bit ("串口调试助手"));
setMinimumSize(650, 600);
setMaximumSize(650, 600);
QStringList serialPortNameList;
serialPortNameList= getSerialNameList();
ui->selectserialComboBox->addItems(serialPortNameList);
ui->selectBuadComboBox->addItems(QStringList()<<"1200"
                                            <<"2400"
                                            <<"4800"
                                            <<"9600"
                                            <<"38400"
                                            <<"25600"
                                            <<"57600"
                                            <<"115200"
                                            <<"256000");
ui->selectBuadComboBox->setCurrentIndex(7);

ui->selectDatabitComboBox->addItems(QStringList() << "5" << "6" << "7" << "8");
ui->selectDatabitComboBox->setCurrentIndex(3);

ui->selectStopbitComboBox->addItem(tr("1"),QSerialPort::OneStop);
ui->selectStopbitComboBox->addItem(tr("1.5"),QSerialPort::OneAndHalfStop);
ui->selectStopbitComboBox->addItem(tr("2"),QSerialPort::TwoStop);


ui->selectTestbitComboBox->addItem(QString::fromLocal8Bit ("奇校验"),QSerialPort::EvenParity);
ui->selectTestbitComboBox->addItem(QString::fromLocal8Bit ("偶校验"),QSerialPort::OddParity);
ui->selectTestbitComboBox->addItem(QString::fromLocal8Bit ("无校验"),QSerialPort::NoParity);
ui->selectTestbitComboBox->setCurrentIndex(2);

ui->radioButtonTextR->setChecked(true);
ui->lineEditInterval->setEchoMode(QLineEdit::Normal);
ui->lineEditInterval->setText("1000");

if (ui->openCloseSerialPushButton->text()==QString::fromLocal8Bit ("打开串口")){
    ui->SenddatapushButton->setEnabled(false);
    ui->checkBoxTimer->setEnabled(false);
  }
else{
     ui->SenddatapushButton->setEnabled(true);
     ui->checkBoxTimer->setEnabled(true);
 }
ui->lineEditInterval->setValidator(new QIntValidator(0,1000000,this));
ui->SenddatalineEdit->setPlaceholderText("input a-zA-Z0-9");

statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 设置不显示label的边框
QLabel *per1 = new QLabel("TX:", this);

QLabel *per3 = new QLabel("RX:", this);

statusBar()->addPermanentWidget(per3,1); //现实永久信息
statusBar()->addPermanentWidget(ui->labelRXnum,1);
statusBar()->addPermanentWidget(per1,1);
statusBar()->addPermanentWidget(ui->labelTXnum,1);


}


void Seriport::createActions()
{

}


QStringList Seriport::getSerialNameList() {

  QStringList list;

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
     //  serial = new  QSerialPort() ;
        QSerialPort serial;
        serial.setPort(info);
                if (serial.open(QIODevice::ReadWrite)){
                  //  ui->selectserialComboBox->addItem(info.portName());
                    ui->ReceivetextEdit->append(info.portName());
                    ui->ReceivetextEdit->append(info.description());
                    ui->ReceivetextEdit->append(info.manufacturer());
//                    qDebug()<<info.portName();
//                    qDebug()<<info.description();
//                    qDebug()<<info.manufacturer();
                    serial.close();
                }
                list.append(info.portName());
        }

   return list;
}

void Seriport::on_openCloseSerialPushButton_clicked()
{
    QString parity;
    if(ui->openCloseSerialPushButton->text() ==QString::fromLocal8Bit ("打开串口"))
        {
    ui->SenddatapushButton->setEnabled(true);
    ui->selectserialComboBox->setEnabled(false);
    ui->checkBoxTimer->setEnabled(true);

    Myserial = new QSerialPort();

    Myserial->setPortName(ui->selectserialComboBox->currentText());
    Myserial->open(QIODevice::ReadWrite);
    Myserial->setBaudRate(ui->selectBuadComboBox->currentText().toInt());
  //  Myserial->setDataBits(ui->selectDatabitComboBox->currentText().toInt());

    switch(ui->selectStopbitComboBox->currentIndex()){
    case 0:
          Myserial->setStopBits(QSerialPort::OneStop);
        break;
    case 1 :
       Myserial->setStopBits(QSerialPort::OneAndHalfStop);
       ui->ReceivetextEdit->append(ui->selectStopbitComboBox->currentText());
        break;
    case 2:
        Myserial->setStopBits(QSerialPort::TwoStop);
       break;
    default:
       break;
    }
    switch (ui->selectDatabitComboBox->currentIndex()){//数据位选择
    case 0:
        Myserial->setDataBits(QSerialPort::Data5);break;
    case 1:
        Myserial->setDataBits(QSerialPort::Data6);break;
    case 2:
        Myserial->setDataBits(QSerialPort::Data7);break;
    case 3:
        Myserial->setDataBits(QSerialPort::Data8);break;
     default :
              break;
    }
    qDebug()<<ui->selectDatabitComboBox->currentText();

    switch (ui->selectTestbitComboBox->currentIndex()) {//校验位选择

    case 0:
        Myserial->setParity(QSerialPort::EvenParity);
        parity = QString::fromLocal8Bit ("奇校验");
       break;
    case 1:
        Myserial->setParity(QSerialPort::OddParity);
         parity = QString::fromLocal8Bit ("偶校验");
        break;
    case 2:
        Myserial->setParity(QSerialPort::NoParity);
          parity = QString::fromLocal8Bit ("无校验");
        break;
    default:
        break;
    }
    qDebug()<<ui->selectTestbitComboBox->currentText();
   // Myserial->setParity(ui->selsetTestbitComboBox->currentIndex());
     Myserial->setFlowControl(QSerialPort::NoFlowControl);

    connect(Myserial,SIGNAL(readyRead()),this,SLOT(readSetting()));

    ui->openCloseSerialPushButton->setText(QString::fromLocal8Bit ("关闭串口"));
    }
    else{
        ui->SenddatapushButton->setEnabled(false);
        ui->selectserialComboBox->setEnabled(true);
        ui->checkBoxTimer->setEnabled(false);

        Myserial->clear();
        Myserial->close();
        Myserial->deleteLater();
        ui->openCloseSerialPushButton->setText(QString::fromLocal8Bit ("打开串口"));
    }
    QString lable = Myserial->portName()+","+QString::number(Myserial->baudRate())+"bps,"
            +","+parity+","+QString::number(Myserial->dataBits())+","+QString::number(Myserial->stopBits());
    QLabel *LableStatus = new QLabel(lable,this);
    statusBar()->addPermanentWidget(LableStatus,3);
  //  statusBar()->insertPermanentWidget(2, LableStatus,8);
}

void Seriport::on_ClearDataPushbutton_clicked()
{
    ui->ReceivetextEdit->clear();
}

void Seriport::readSetting()
{
   QByteArray requestData;

   requestData = Myserial->readAll();

//   qDebug()<<requestData.size();
//   qDebug()<<requestData;

   int  rxdata = requestData.size();
   static int rxdata1;
   rxdata1+= rxdata;

   ui->labelRXnum->setText(QString::number(rxdata1));

  // ui->ReceivetextEdit->moveCursor(QTextCursor::EndOfLine);
   if(ui->radioButtonTextR->isChecked())
   {
       ui->ReceivetextEdit->insertPlainText(requestData);
      // qDebug() << "receive text";
   }else if(ui->radioButtonHexR->isChecked())
   {
//第一种方法转换十六进制
//               QDataStream out(&requestData,QIODevice::ReadWrite);    //将字节数组读入
//        ;
//                while(!out.atEnd())
//           {
//
//                   qint8 outChar = 0;
//                   out>>outChar;    //每字节填充一次，直到结束
//                   //十六进制的转换
//                   QString str = QString("%1").arg(outChar & 0xFF,2,16,QLatin1Char('0'));
//                  ui->ReceivetextEdit->insertPlainText(str.toUpper());//大写
//                  ui->ReceivetextEdit->insertPlainText(" ");//每发送两个字符后添加一个空格
//                  ui->ReceivetextEdit->moveCursor(QTextCursor::End);
//               }
//                qDebug()<<hexData;
//
//第二种方法转换十六进制
         QString strDis;
         QString str = requestData.toHex();//以十六进制显示
         qDebug()<<str;
                  str = str.toUpper ();//转换为大写
                  for(int i = 0;i<str.length ();i+=2)//填加空格
                  {
                         QString st = str.mid (i,2);
                         strDis += st;
                         strDis += " ";
                   }
       //  ui->ReceivetextEdit->insertPlainText(requestData.toHex());
          ui->ReceivetextEdit->insertPlainText(strDis);
          ui->ReceivetextEdit->insertPlainText(tr("\n"));

     }
requestData.clear();

}

void Seriport::on_SenddatapushButton_clicked()
{
    QString sendData = ui->SenddatalineEdit->text();



    if(ui->openCloseSerialPushButton->text()==QString::fromLocal8Bit ("关闭串口") && !ui->SenddatalineEdit->text().isEmpty())
        {
       if(ui->checkBoxAddRow->checkState()){
                 qDebug()<<"add a row";
                 sendData +="\r\n";
        }
          if (ui->hexSentcheckBox->checkState())
          {
              int TXdata;
              static int TXdata1;
              if (sendData.size()%2!=0)
              {
                  TXdata = sendData.size()/2+1;
                  TXdata1+= TXdata;
                   ui->labelTXnum->setText(QString::number(TXdata1));
                   sendData = QString::number(0)+sendData;
              }
              else{
                  TXdata = sendData.size()/2;
                  TXdata1+= TXdata;
                  ui->labelTXnum->setText(QString::number(TXdata1));
              }
           QByteArray str = QByteArray::fromHex (sendData.toLatin1().data());
           Myserial->write(str);
          }
          else {

                Myserial->write(sendData.toLatin1());
                int TXdata = sendData.size();
                static int TXdata1;
                TXdata1+= TXdata;
                ui->labelTXnum->setText(QString::number(TXdata1));
               }

        }

   else{
        QMessageBox::information(this,QString::fromLocal8Bit ("警告"),QString::fromLocal8Bit ("请首先打开一个串口且发送不为空"));
    }
}

void Seriport::sendData()
{
    QString sendData = ui->SenddatalineEdit->text();
    if(ui->openCloseSerialPushButton->text()==QString::fromLocal8Bit ("关闭串口") && !ui->SenddatalineEdit->text().isEmpty())
        {
//        if(ui->checkBoxTimer->checkState()){
//            qDebug()<<"timer";
//        }
       if(ui->checkBoxAddRow->checkState()){
                 qDebug()<<"add a row";
                 sendData +="\r\n";

        }
          if (ui->hexSentcheckBox->checkState())
          {
            QByteArray str = QByteArray::fromHex (sendData.toLatin1().data());
            Myserial->write(str);

          }
          else {
                Myserial->write(sendData.toLatin1());
               }
        }
   else{
        QMessageBox::information(this,QString::fromLocal8Bit ("警告"),QString::fromLocal8Bit ("请首先打开一个串口且发送不为空"));
    }
}
void Seriport::on_QuitpushButton_clicked()
{
    QApplication* app;
    app->exit(0);
}

void Seriport::on_checkBoxTimer_toggled(bool checked)
{
    if (!ui->SenddatalineEdit->text().isEmpty()){
      if (checked){
          ui->openCloseSerialPushButton->setEnabled(false);
          ui->lineEditInterval->setEnabled(false);
          autoSendTimer = new QTimer();
          connect(autoSendTimer, SIGNAL(timeout()), this, SLOT(on_SenddatapushButton_clicked()));
          int time = ui->lineEditInterval->text().toInt();
          autoSendTimer->start(time);
      }

      else{
          autoSendTimer->stop();
          ui->openCloseSerialPushButton->setEnabled(true);
          ui->lineEditInterval->setEnabled(true);
          }
      }
    else{
        ui->checkBoxTimer->setChecked(false);

        QMessageBox::information(this,QString::fromLocal8Bit ("警告"),QString::fromLocal8Bit ("发送不能为空"));

    }
}


void Seriport::on_hexSentcheckBox_toggled(bool checked)
{
    if(checked)
    {
        QRegExp abcd("[a-fA-F0-9]+$");
         ui->SenddatalineEdit->setValidator(new QRegExpValidator(abcd));
         ui->SenddatalineEdit->setPlaceholderText("input a-fA-F0-9");
    }else{
         ui->SenddatalineEdit->setValidator(new QRegExpValidator());
         ui->SenddatalineEdit->setPlaceholderText("input a-zA-Z0-9");
    }

}

void Seriport::on_pushButtonSaveData_clicked()
{
   QString filePath = "./data.txt";

        QFile file(filePath);
     //   if (file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
                QString data = ui->ReceivetextEdit->document()->toPlainText();
                stream << data;

           }
            else {
                QString title = title.fromLocal8Bit("文件打开错误");
                QString information = information.fromLocal8Bit("请在当前目录下创建data.txt");
                QMessageBox::warning(this, title, information,QMessageBox::Ok);
            }
       // }
//        else {
//            QMessageBox::warning(this, "文件打开错误", "请手动在相对路径下 '-- '创建文件'data.txt'", QMessageBox::Ok);
//       }
     }
