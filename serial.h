#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QComboBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextCodec>
#include <QTextBrowser>
#include <QMessageBox>

#include <QDebug>

namespace Ui {
class Serial;
}

class Serial : public QWidget
{
    Q_OBJECT

public:
    explicit Serial(QWidget *parent = 0);
    ~Serial();
    void SearchSerial(QComboBox *combox);
    bool SerialSendStr(QString &textbuff);
    void UpdateSerialInfo();
    void setBaud(long value);
    void SerialOpen(int index);
    void SerialClose();
    int GetSerialStatus();

    void SerialInit(QTextBrowser *box);
    QString getStrtext() const;

signals:
    void ReceiveSerial(const QString &);
    void Openserial();

private slots:
    void SerialRead();

private:
    Ui::Serial *ui;
    static int status;
    static long baud;
    static int SerialIsOpen;
    static QStringList portlist;
    static int ReadStrstatus;
    QSerialPort *serial;

    QByteArray dataTemp;
    QString strtext;
    QTextBrowser *debugbox;
};

#endif // SERIAL_H
