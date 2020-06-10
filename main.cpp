#include "mainwindow.h"

#include <QApplication>
//导入串口包
#include <QCoreApplication>
#include <stdio.h>
#include <QDebug>

#include <QIcon>
#include <QMessageBox>
#include "connector.h"
#include "SingleApplication.h"
#include "MyQWidget.h"

int main(int argc, char *argv[]) {
    SingleApplication app(argc, argv);
    if(app.isRunning()) {
       QMessageBox::information(NULL, QStringLiteral("提示"), QStringLiteral("程序已运行！"));
       return 0;
    }
    qDebug()<< "main.cpp执行了";
    app.setWindowIcon(QIcon(":/img/gangtiexia.png"));
    //new Connector();
    MyQWidget myQWidget;
    myQWidget.show();
    return app.exec();
}
