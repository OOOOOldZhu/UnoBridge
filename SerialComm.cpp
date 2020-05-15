
#include "SerialComm.h"

#include <QString>
#include <QObject>
#include <iostream>
#include <memory>
#include <QList>
#include <stdio.h>
#include <QDebug>
#include <QByteArray>

//串口通讯
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

SerialComm::SerialComm(Connector *connector){
    this->connector = connector;
}

void SerialComm::init(){
    qDebug()<<"1.initSerial() 执行了";
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    QSerialPortInfo selectedPortInfo;
    for(int i = 0;i < serialList.length();i++){
        QSerialPortInfo itemInfo = serialList[i];
        //qDebug()<< "< - - - - - - - - - "<<i<<"- - - - - - - - - - - - - - - - -  >";
        //输出链表头节点的一些信息
        //printPort(itemInfo);
        bool a = itemInfo.manufacturer().contains("Arduino");
        bool b = itemInfo.portName().indexOf("cu") >= 0;
        //qDebug()<<a<<" = = = "<<b;
        if( a && b){
            selectedPortInfo = itemInfo;
            break;
        }
    }
    printPort(selectedPortInfo);

    if(!selectedPortInfo.isNull()){
        QSerialPort *serial = new QSerialPort;
        //设置串口名 cu.usbmodem142401
        serial->setPortName(selectedPortInfo.portName());
        //connect(serial,&QSerialPort::readyRead,this,&Connector::ReadData);
        //QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));
        serial->setBaudRate(QSerialPort::Baud57600);  //波特率
        serial->setDataBits(QSerialPort::Data8);     //数据位
        serial->setParity(QSerialPort::NoParity);    //无奇偶校验
        serial->setStopBits(QSerialPort::OneStop);   //无停止位
        serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
        bool isOpen = serial->open(QIODevice::ReadWrite);    //读写打开

        qDebug()<<"串口已经打开打开 = "<<isOpen;
        if(isOpen){
            this->serial=serial;
            QObject::connect(serial,&QSerialPort::readyRead,this,&SerialComm::readData);
            //串口主动调用，才会执行这个回调函数
            //QObject::connect(serial,&QSerialPort::aboutToClose,this,&Connector::onDisconnected);
            if(connector)connector->socketSendMsg("suc msg: initSerial success");
        }
    }else{
        qDebug()<< "串口集合为空 ~";
        if(connector)connector->socketSendMsg("err msg: arduino serialport null");
    }

}

//串口数据返回
void SerialComm::readData(){
    //qDebug()<<"串口readData槽函数";
    if(this->serial){
        QByteArray buf = this->serial->readAll();
        QByteArray array =buf.toHex();
        QString str = QString(array);

        qDebug()<<"串口接收到的buf.str = "<<str;
        if(connector)connector->socketSendMsg(str);
    }
}


void SerialComm::send(QByteArray arr){
    //this->serial->write(arr);
    this->serial->write(arr);
}

void SerialComm::closeComm(){
    this->serial->close();
    this->serial->clear();
}

void SerialComm:: onDisconnected(){
    qDebug()<<"串口已经关闭";
    if(connector)connector->socketSendMsg("err msg: serialport close");
}

void SerialComm::printPort(QSerialPortInfo itemInfo){
    if(itemInfo.isNull()){
        return;
    }
    qDebug()<< "Name : "<<itemInfo.portName();
    qDebug()<< "Description : "<<itemInfo.description();
    qDebug()<< "Manufacturer: "<<itemInfo.manufacturer();
    qDebug()<< "Serial Number: "<<itemInfo.serialNumber();
    qDebug()<< "System Location: "<<itemInfo.systemLocation();
}
