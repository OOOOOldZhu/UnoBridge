#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>
#include <QtSerialPort/QSerialPort>
#include <IToBoardComm.h>

class Connector;
//智能指针 https://www.cnblogs.com/xudong-bupt/p/9027609.html
//typedef std::shared_ptr<QWebSocketServer> QWebSocketServerPtr;
//typedef std::shared_ptr<QWebSocket> QWebSocketPtr;


class Connector: public QObject{
    //成员变量
    Q_OBJECT
    QWebSocketServer *server;
    QList<QWebSocket*> soketList;

    IToBoardComm *toBoardComm;
    //构造函数
public:
    Connector();
    void socketSendMsg(QString str);

    //内部函数
private:
    void initWebsocket();
    void onNewConnection();
    void onReceivedMsg(const QString& message);
    void socketDisconnected();






//signals:
//        void readyRead();
//private slots:
//    void readData();






};

#endif // CONNECTOR_H
