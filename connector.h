#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>
#include <QtSerialPort/QSerialPort>

//串口导包
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo.h>
#include <QLowEnergyController.h>

class Connector;
//智能指针 https://www.cnblogs.com/xudong-bupt/p/9027609.html
//typedef std::shared_ptr<QWebSocketServer> QWebSocketServerPtr;
//typedef std::shared_ptr<QWebSocket> QWebSocketPtr;


class Connector: public QObject{
    //成员变量
    Q_OBJECT
    QSerialPort *serial;
    QWebSocketServer *server;
    QList<QWebSocket*> soketList;

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *bleController;
    //构造函数
public:
    Connector();

    //内部函数
private:
    void initWebsocket();
    void onNewConnection();
    void onReceivedMsg(const QString& message);
    void socketDisconnected();
    void socketSendMsg(QString str);

    void initSerial();
    void readData();
    void onDisconnected();
    void printPort(QSerialPortInfo itemInfo);

    void initBle();
    void onFindBleDevice(const QBluetoothDeviceInfo &info);
    void onScanFinished();
    void onScanCanceled();
    void serviceDiscovered(const QBluetoothUuid &newService);
    void serviceScanDone();
    void onBleConneErr(QLowEnergyController::Error newError);
    void deviceConnected();
    void deviceDisconnected();

//signals:
//        void readyRead();
//private slots:
//    void readData();






};

#endif // CONNECTOR_H
