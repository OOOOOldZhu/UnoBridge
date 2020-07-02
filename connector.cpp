#include "connector.h"

#include <QString>
#include <QObject>
#include <iostream>
#include <memory>
#include <QList>
#include <stdio.h>
#include <QDebug>

//websocket通讯
#include <QWebSocketServer>
#include <QWebSocket>

#include <tool.h>
#include <SerialComm.h>
#include <BleComm.h>

Connector::Connector(){
    //1，连接websocket
    this->initWebsocket();
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
    qInfo() << "socket接收:" << message;
    if(message.contains("initSerial")){
        //网页要求初始化串口
        if(toBoardComm){
           toBoardComm = new SerialComm(this);
           toBoardComm->init();
        }
        return;
    }
    if(message.contains("initBle")){
        //网页请求蓝牙连接
        //if(!toBoardComm){
           toBoardComm = new BleComm(this);
           toBoardComm->init();
        //}
        return;
    }
    if(message.contains("close")){
        if(toBoardComm){
            toBoardComm->closeComm();
            toBoardComm = NULL;
        }
        return;
    }
    //数据转换参考 https://blog.csdn.net/biersibao/article/details/82884719
    QByteArray arr = Tool::tenString2ByteArray2(message);
    qInfo() << "socket接收Hex--" << Tool::ByteArrayToHexString(arr);
    if(toBoardComm){
       toBoardComm->send(arr);
    }
}

void Connector::socketDisconnected()
{
    qInfo() << "socket断开连接";
    if(toBoardComm){
       toBoardComm->closeComm();
    }
}

void Connector::socketSendMsg(QString msg){
    for(int i = 0;i<soketList.length();i++){
        soketList[i]->sendTextMessage(msg);
    }
}





