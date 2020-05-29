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
    QApplication qapplication(argc, argv);
//    QString dir = QApplication::applicationDirPath();
//    QApplication::addLibraryPath("./images");

    qapplication.setWindowIcon(QIcon("gangtiexia.icns"));
//    MainWindow w;
//    w.show();
    qDebug()<< "main.cpp执行了";

    new Connector();

    return qapplication.exec();
}

