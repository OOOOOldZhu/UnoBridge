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
#include <QMessageBox>

//蓝牙通讯
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothLocalDevice>

BleComm::BleComm(Connector *connector){
    this->connector = connector;
}

void BleComm::init(){
    qInfo()<<"initBle() . . .";
    QBluetoothLocalDevice *localDevice = new QBluetoothLocalDevice();
    if( localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff )  {
        qInfo()<<"你的电脑蓝牙是关闭状态!";
        localDevice->powerOn();
        QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("你的电脑蓝牙是关闭状态!请手动打开!"));
        return;
    }else {

    }
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
    qWarning() << "Discovered LE Device name: " << info.name() << " Address: "<< info.address().toString() << " rssi: "<< info.rssi();
    //bool a = info.address()==QBluetoothAddress("B4:4B:0E:04:2D:7A");
    bool a = info.rssi()>-80;
    bool b = info.name().indexOf("HC-42")>-1;
    bool c = a && b;
    if(c){
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

void BleComm::serviceScanDone(){
    qInfo()<<"serviceScanDone ()";
}

void BleComm::serviceDiscovered(const QBluetoothUuid &uuid){
    qInfo()<<"serviceDiscovered() QBluetoothUuid = "<<uuid;
    //{0000ffe0-0000-1000-8000-00805f9b34fb}
    if (uuid == QBluetoothUuid(QBluetoothUuid::HeartRate)) {
        //setMessage("Heart Rate service discovered. Waiting for service scan to be done...");
        //foundHeartRateService = true;
    }
    if(uuid.toString() == "{0000ffe0-0000-1000-8000-00805f9b34fb}"){
        //找到所需要的服务之后，创建serviceObject
        qInfo()<<"11111";
        this->m_Service = bleController->createServiceObject(uuid);
        //如果找到的服务中有所需服务，并匹配、创建成功则进入if语句，链接相应的信号-槽
        if(m_Service){
            qInfo()<<"22222";
            QList<QLowEnergyCharacteristic> list = m_Service->characteristics();
            qInfo()<<"33333";
            QObject::connect(m_Service, &QLowEnergyService::stateChanged, this, &BleComm::SL_serviceStateChanged);//状态改变信号
            QObject::connect(m_Service, &QLowEnergyService::characteristicChanged, this, &BleComm::SL_characteristicChanged);//特性改变信号
            QObject::connect(m_Service, &QLowEnergyService::descriptorWritten, this, &BleComm::SL_descriptorWritten);//描述符写入信号
            m_Service->discoverDetails();//完成之后需要执行此行已发现服务所包含的特性
        }else{
            //QMessageBox::information(this,tr("Info"),"Service not found.");
        }
    }
}

void BleComm::SL_serviceStateChanged(QLowEnergyService::ServiceState newState){
    qInfo()<<"SL_serviceStateChanged() newState = "<<newState;
//    if(newState == QLowEnergyService::ServiceDiscovered) {
//        QLowEnergyCharacteristic hrChar = this->m_Service->characteristic(QBluetoothUuid(quint16(0xfff6)));
//        if(!hrChar.isValid()) {
//            qDebug() << "characteristic fff6 error:::";
//        }
//        // 设置特征对象可用
//        //enable the chracteristic notification by write 0x01 to client characteristic configuration
//        QLowEnergyDescriptor m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//        if (m_notificationDesc.isValid()) {
//            if(hrChar.properties() & QLowEnergyCharacteristic::Notify) {
//                qDebug() << "------";
//            }
//            m_Service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
//        }
//    }
}

void BleComm::SL_characteristicChanged(const QLowEnergyCharacteristic &info,const QByteArray &value){
    qDebug() << "characteristicChanged state change::||||||";
    qDebug() << "value length::" << value.length();
    qDebug() << "value length::" << value;

    QByteArray sub_1 = value.mid(0,2);
    QByteArray sub_2 = value.mid(2,2);
    QByteArray sub_3 = value.mid(4,2);

    bool ok;
    // num 1,2,3  分别对应三个压力感应点的压力值
    int num_1 =  QString(sub_1.toHex()).toInt(&ok,16);
    qDebug() << "num_1:::" << num_1;
    int num_2 =  QString(sub_2.toHex()).toInt(&ok,16);
    qDebug() << "num_1:::" << num_2;
    int num_3 =  QString(sub_3.toHex()).toInt(&ok,16);
    qDebug() << "num_1:::" << num_3;
}

void BleComm::SL_descriptorWritten(const QLowEnergyDescriptor &info,const QByteArray &value){
    qDebug() << "SL_descriptorWritten() = ";
}

void BleComm::send(QByteArray arr){
    //qInfo()<<"deviceDisconnected ()";
}

void BleComm::closeComm(){
    //qInfo()<<"deviceDisconnected ()";
    if(bleController){
        bleController->disconnectFromDevice();
    }
}

