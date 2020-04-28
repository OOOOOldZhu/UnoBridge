#include "connector.h"

#include <QDebug>
#include <QCoreApplication>
#include <QList>
#include <stdio.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

Connector::Connector(){
    //1，先创建串口连接
    this->initSerial();
}

void Connector::initSerial(){
    //qDebug()<<"1.initSerial() 执行了";
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    QSerialPortInfo selectedPortInfo;
    for(int i = 0;i < serialList.length();i++){
        QSerialPortInfo itemInfo = serialList[i];
        qDebug()<< "< - - - - - - - - - "<<i<<"- - - - - - - - - - - - - - - - -  >";
        //输出链表头节点的一些信息
        qDebug()<< "Name : "<<itemInfo.portName();
        qDebug()<< "Description : "<<itemInfo.description();
        qDebug()<< "Manufacturer: "<<itemInfo.manufacturer();
        qDebug()<< "Serial Number: "<<itemInfo.serialNumber();
        qDebug()<< "System Location: "<<itemInfo.systemLocation();
        bool a = itemInfo.manufacturer().contains("Arduino");
        bool b = itemInfo.portName().indexOf("cu") >= 0;
        qDebug()<<a<<" = = = "<<b;
        if( a && b){
            selectedPortInfo = itemInfo;
            break;
        }
    }
    qDebug()<<"被选中的串口 = "<< selectedPortInfo.portName();
    if(!selectedPortInfo.isNull()){
        QSerialPort *serial = new QSerialPort;
        //设置串口名 cu.usbmodem142401
        serial->setPortName(selectedPortInfo.portName());
        //connect(serial,&QSerialPort::readyRead,this,&Connector::ReadData);
        //QObject::connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));
        serial->setBaudRate(QSerialPort::Baud115200);  //波特率
        serial->setDataBits(QSerialPort::Data8);     //数据位
        serial->setParity(QSerialPort::NoParity);    //无奇偶校验
        serial->setStopBits(QSerialPort::OneStop);   //无停止位
        serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
        bool isOpen = serial->open(QIODevice::ReadWrite);    //读写打开

        qDebug()<<"串口已经打开打开 = "<<isOpen;
        if(isOpen){
            QObject::connect(serial,&QSerialPort::readyRead,this,&Connector::ReadData);
        }
    }

}

//串口数据返回
void Connector::ReadData(){
    QByteArray buf = this->serial->readAll();
    QString str = QString(buf);
    qDebug()<<"接收到的buf     = "<<buf;
    qDebug()<<"接收到的buf.str = "<<str;
}



