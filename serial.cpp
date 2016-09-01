#include "serial.h"

long Serial::baud = 9600;
QStringList Serial::portlist;
int Serial::status = 0;
int Serial::ReadStrstatus = 0;

Serial::Serial(QWidget *parent) :
    QWidget(parent)
{

}

Serial::~Serial()
{

}

void Serial::SearchSerial(QComboBox *combox)
{
    QStringList lists;
    Serial::portlist.clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QString portname = info.portName();
        QString description = info.description();
        QString manufacturer = info.manufacturer();
        QString comportinfo = description + " " +portname;
        Serial::portlist << portname;
        lists << comportinfo;

        qDebug() << "Name        : " << portname;
        qDebug() << "Description : " << description;
        qDebug() << "Manufacturer: " << manufacturer;
        debugbox->append("Name        : " + portname);
        debugbox->append("Description : " + description);
        debugbox->append("Manufacturer: " + manufacturer);
        //debugbox->setCursor(QTextCursor::End);
    }
    combox->clear();
    combox->addItems(lists);
}

bool Serial::SerialSendStr(QString &textbuff)
{
    int err;
    if (Serial::status)
    {
        serial->write(textbuff.toLocal8Bit().data());
        return true;
    }
    else{
        debugbox->append("串口没有打开");
        err = QMessageBox::information(this,"warning","串口没有打开,是否打开？",QMessageBox::Yes,QMessageBox::No);
        if (err == QMessageBox::Yes)
        {
            emit Openserial();
        }
        return false;
      //  debugbox->setCursor(QTextCursor::End);
    }
       // qDebug()<<"串口没有打开";
}

void Serial::setBaud(long value)
{
    Serial::baud = value;
    qDebug()<<Serial::baud;
    if (Serial::status)
    {
        switch(Serial::baud)
        {
            case 1200:serial->setBaudRate(QSerialPort::Baud1200);break;
            case 2400:serial->setBaudRate(QSerialPort::Baud2400);break;
            case 4800:serial->setBaudRate(QSerialPort::Baud4800);break;
            case 9600:serial->setBaudRate(QSerialPort::Baud9600);break;
            case 19200:serial->setBaudRate(QSerialPort::Baud19200);break;
            case 57600:serial->setBaudRate(QSerialPort::Baud57600);break;
            case 115200:serial->setBaudRate(QSerialPort::Baud115200);break;
        default: debugbox->append("波特率设置错误");break;
        }
    }
}

void Serial::SerialRead()
{
    while (!serial->atEnd()){
        dataTemp = serial->readAll();
        QTextCodec *cod = QTextCodec::codecForName("GBK");
        strtext = cod->toUnicode(dataTemp);
    }
    Serial::ReadStrstatus = 1;
    emit ReceiveSerial(strtext);
}

QString Serial::getStrtext() const
{
    if (Serial::ReadStrstatus)
    {
        Serial::ReadStrstatus = 0;
        return strtext;
    }
    return "0";
}

void Serial::SerialOpen(int index)
{
    long baudtemp = Serial::baud;
    if (!Serial::status)
    {
        if (portlist.size()){
            serial = new QSerialPort(portlist.at(index));
            serial->open(QIODevice::ReadWrite);//要先打开才能进行一下操作
            //serial->setBaudRate(QSerialPort::Baud9600);
            setBaud(baudtemp);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            connect(serial,SIGNAL(readyRead()),this,SLOT(SerialRead()));

            Serial::status = 1;
        }
        else
            Serial::status = 0;
    }
    else{
        debugbox->append("串口已经打开");
//        debugbox->setCursor(QTextCursor::End);
    }
        //qDebug()<<"串口已经打开";
}

void Serial::SerialClose()
{
    serial->close();
    debugbox->append("串口已经关闭");
//    debugbox->setCursor(QTextCursor::End);
    Serial::status = 0;
}

int Serial::GetSerialStatus()
{
    return Serial::status;
}

void Serial::SerialInit(QTextBrowser *box)
{
    debugbox = box;
}


