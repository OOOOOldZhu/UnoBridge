#include "connector.h"

#include <QString>

//websocket通讯
#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>

//串口通讯
#include <QDebug>
#include <QCoreApplication>
#include <QList>
#include <stdio.h>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <tool.h>

Connector::Connector(){
    //1，连接websocket
    this->initWebsocket();
    //2，先创建串口连接
    //this->initSerial();
}

void Connector::initWebsocket(){
    QWebSocketServer *server = new QWebSocketServer(QStringLiteral("Inspection server"),QWebSocketServer::NonSecureMode);

    this->server=server;
    if(server->listen(QHostAddress::Any, 1234))
    {
        QObject::connect(server,&QWebSocketServer::newConnection,this,&Connector::onNewConnection);
    }
}

void Connector::onNewConnection(){
    qInfo() << "socket onNewConnection()";
    QWebSocket *socket = this->server->nextPendingConnection();
    QObject::connect(socket, &QWebSocket::textMessageReceived, this, &Connector::onReceivedMsg);
    QObject::connect(socket, &QWebSocket::disconnected, this, &Connector::socketDisconnected);

//    for(int i = 0;i<soketList.length();i++){
//        soketList[i]->close();
//    }
    soketList.clear();
    this->soketList.append(socket);
}

void Connector::onReceivedMsg(const QString& message)
{
    if(message.contains("initSerial")){
        //网页要求初始化串口
        this->initSerial();
        return;
    }
    //数据转换参考 https://blog.csdn.net/biersibao/article/details/82884719
    QByteArray arr = Tool::tenString2ByteArray2(message);
    qInfo() << "socket接收:" << Tool::ByteArrayToHexString(arr);
    if(serial){
       serial->write(arr);
    }
}

void Connector::socketDisconnected()
{
    qInfo() << "socket断开连接";
    this->serial->close();
    this->serial->clear();
}


void Connector::initSerial(){
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
            QObject::connect(serial,&QSerialPort::readyRead,this,&Connector::readData);
            socketSendMsg("suc msg: initSerial success");
        }
    }else{
        qDebug()<< "串口集合为空 ~";
        socketSendMsg("err msg: arduino serialport null");
    }

}

//串口数据返回
void Connector::readData(){
    //qDebug()<<"串口readData槽函数";
    if(this->serial){
        QByteArray buf = this->serial->readAll();
        QByteArray array =buf.toHex();
        QString str = QString(array);

        qDebug()<<"串口接收到的buf.str = "<<str;
        socketSendMsg(str);
    }
}

void Connector::socketSendMsg(QString msg){
    for(int i = 0;i<soketList.length();i++){
        soketList[i]->sendTextMessage(msg);
    }
}

void Connector::printPort(QSerialPortInfo itemInfo){
    if(itemInfo.isNull()){
        return;
    }
    qDebug()<< "Name : "<<itemInfo.portName();
    qDebug()<< "Description : "<<itemInfo.description();
    qDebug()<< "Manufacturer: "<<itemInfo.manufacturer();
    qDebug()<< "Serial Number: "<<itemInfo.serialNumber();
    qDebug()<< "System Location: "<<itemInfo.systemLocation();
}


