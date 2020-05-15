#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <IToBoardComm.h>
#include <connector.h>

class SerialComm : public IToBoardComm
{

    Q_OBJECT
    QSerialPort *serial;
    Connector *connector;

public:
    SerialComm(Connector *connector);
    void init() override;
    void send(QByteArray arr) override;
    void closeComm() override;

private:
    void readData();
    void onDisconnected();
    void printPort(QSerialPortInfo itemInfo);
};



#endif // SERIALCOMM_H
