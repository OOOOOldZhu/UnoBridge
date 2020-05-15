#include "BleComm.h"

#include <QString>
#include <QObject>
#include <iostream>
#include <memory>
#include <QList>
#include <stdio.h>
#include <QDebug>
#include <QDateTime>
#include <QByteArray>

//蓝牙通讯
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>

BleComm::BleComm(Connector *connector){
    this->connector = connector;
}

void BleComm::init(){
    qInfo()<<"initBle() . . .";
    //参考：https://blog.csdn.net/wqwqwq604/article/details/93198438
    //创建搜索服务
    QBluetoothDeviceDiscoveryAgent *discoveryAgent =new QBluetoothDeviceDiscoveryAgent();
    this->discoveryAgent = discoveryAgent;
    //设置BLE的搜索时间
    //discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    //找到设备之后添加到蓝牙列表中
    //connect(discoveryAgent,SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),this,SLOT(onFindBleDevices(QBluetoothDeviceInfo)));
    //connect(discoveryAgent, SIGNAL(finished()), this, SLOT(onScanFinished()));
    //connect(discoveryAgent, SIGNAL(canceled()), this, SLOT());
    QObject::connect(discoveryAgent,&QBluetoothDeviceDiscoveryAgent::deviceDiscovered,this,&BleComm::onFindBleDevice);
    //QObject::connect(discoveryAgent,&QBluetoothDeviceDiscoveryAgent::finished,this,&Connector::onScanFinished);
    //QObject::connect(discoveryAgent,&QBluetoothDeviceDiscoveryAgent::canceled,this,&Connector::onScanCanceled);
    discoveryAgent->start();
}

void BleComm::onFindBleDevice(const QBluetoothDeviceInfo &info){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    qInfo()<<"- - - - - - - - - - - - "+current_date+"- - - - - - - - - - - - - - - - - - ";
//    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
//            qWarning() << "Discovered LE Device name: " << info.name() << " Address: "<< info.address().toString();
//    }
    bool b ;
    b= info.address()==QBluetoothAddress("B4:4B:0E:04:2D:7A");
    //b = info.name().indexOf("mCookie")>-1;
    if(b){
        qInfo()<<"name ="+info.name();
        qInfo()<<"rssi ="+info.rssi();
        qInfo()<<"address="+info.address().toString();
        discoveryAgent->stop();

        bleController = QLowEnergyController::createCentral(info);//创建中央控制器
        QObject::connect(bleController, &QLowEnergyController::connected,this, &BleComm::deviceConnected);
        QObject::connect(bleController, &QLowEnergyController::disconnected,this, &BleComm::deviceDisconnected);
        QObject::connect(bleController, &QLowEnergyController::serviceDiscovered,this, &BleComm::serviceDiscovered);
        QObject::connect(bleController, &QLowEnergyController::discoveryFinished,this, &BleComm::serviceScanDone);
        //QObject::connect(bleController, &QLowEnergyController::error,this, &Connector::onBleConneErr);
        //QObject::connect(bleController, SIGNAL(error(QLowEnergyController::Error)),this, SLOT(onBleConneErr(QLowEnergyController::Error)));
        bleController->connectToDevice();//建立连接
    }
}
void BleComm::onScanFinished(){
    qInfo() << "ble onScanFinished";
}
void BleComm::onScanCanceled(){
    qInfo() << "ble onScanCanceled";
}
void BleComm::serviceDiscovered(const QBluetoothUuid &gatt){
    qInfo()<<"QBluetoothUuid = "<<gatt;
    if (gatt == QBluetoothUuid(QBluetoothUuid::HeartRate)) {
            //setMessage("Heart Rate service discovered. Waiting for service scan to be done...");
            //foundHeartRateService = true;
     }
}
void BleComm::serviceScanDone(){
    qInfo()<<"serviceScanDone ()";
}
void BleComm::onBleConneErr(QLowEnergyController::Error newError){
    qInfo()<<"onBleConneErr ()"<<newError;
}
void BleComm::deviceConnected(){
    qInfo()<<"deviceConnected ()";
    bleController->discoverServices();
}
void BleComm::deviceDisconnected(){
    qInfo()<<"deviceDisconnected ()";
}




void BleComm::send(QByteArray arr){
    //qInfo()<<"deviceDisconnected ()";
}

void BleComm::closeComm(){
    //qInfo()<<"deviceDisconnected ()";
}

