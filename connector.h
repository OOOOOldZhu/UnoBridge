#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>
#include <QtSerialPort/QSerialPort>



class Connector;
class Connector: public QObject{
    //成员变量
    Q_OBJECT
    QSerialPort *serial;

    //构造函数
public:
    Connector();

    //内部函数
private:
    void initSerial();
    void ReadData();
};

//signals:
//    void readyRead();
//private slots:
//    void ReadData();

#endif // CONNECTOR_H
