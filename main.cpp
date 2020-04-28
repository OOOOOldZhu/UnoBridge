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

#include "connector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    qDebug()<< "main.cpp执行了";

    new Connector();
    //InspectionServer server(1234,serial);

    return a.exec();
}

