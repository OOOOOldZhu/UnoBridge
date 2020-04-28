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
    qDebug()<<"initSerial() 执行了";
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    //输出链表头节点的一些信息
    qDebug()<< "Name : "<<serialList.first().portName();
    qDebug()<< "Description : "<<serialList.first().description();
    qDebug()<< "Manufacturer: "<<serialList.first().manufacturer();
    qDebug()<< "Serial Number: "<<serialList.first().serialNumber();
    qDebug()<< "System Location: "<<serialList.first().systemLocation();

    QSerialPort *serial = new QSerialPort;
    //设置串口名
    //serial->setPortName(serialList.first().portName());
    serial->setPortName(serialList.first().systemLocation());
    connect(serial,&QSerialPort::readyRead,this,&Connector::ReadData);
    serial->setBaudRate(QSerialPort::Baud115200);  //波特率
    serial->setDataBits(QSerialPort::Data8);     //数据位
    serial->setParity(QSerialPort::NoParity);    //无奇偶校验
    serial->setStopBits(QSerialPort::OneStop);   //无停止位
    serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
    bool isOpen = serial->open(QIODevice::ReadWrite);    //读写打开

    qDebug()<<"串口是否打开 = "<<isOpen;
}

//串口数据返回
void Connector::ReadData(){
    QByteArray buf;
    buf = this->serial->readAll();
    QString str = QString(buf);
    qDebug()<<buf;
}



