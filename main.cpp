#include "mainwindow.h"

#include <QApplication>
//导入串口包
#include <QCoreApplication>
#include <QList>
#include <stdio.h>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

//创建websocket
#include <QWebSocketServer>
#include "inspection_server.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    qDebug()<< "main.cpp执行了";

    //实例化一个类型参数为QSerialPortInfo模板类链表
    QList<QSerialPortInfo> serialList;
    //给链表赋值为获取到的当前设备的所有串口信息的链表
    serialList = QSerialPortInfo::availablePorts();
    //输出链表头节点的一些信息
    qDebug()<< "Name : "<<serialList.first().portName();
//    qDebug()<< "Description : "<<serialList.first().description();
//    qDebug()<< "Manufacturer: "<<serialList.first().manufacturer();
//    qDebug()<< "Serial Number: "<<serialList.first().serialNumber();
//    qDebug()<< "System Location: "<<serialList.first().systemLocation();
    QSerialPort *serial = new QSerialPort;
    //设置串口名
    serial->setPortName(serialList.first().portName());
    //打开串口
    serial->open(QIODevice::ReadWrite);
    //设置波特率
    serial->setBaudRate(57600);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
     //设置奇偶校验
     serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);

    //InspectionServer server(1234);

    return a.exec();
}

