#include "mainwindow.h"

#include <QApplication>
//导入串口包
#include <QCoreApplication>
#include <stdio.h>
#include <QDebug>


#include <QIcon>
#include "connector.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug()<< "main.cpp执行了";
    app.setWindowIcon(QIcon("gangtiexia.icns"));
    new Connector();

    return app.exec();
}

