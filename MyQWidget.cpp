#include "MyQWidget.h"
#include<QTextCodec>
#include<QIcon>
#include<QPushButton>

MyQWidget::MyQWidget()

{
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));

    this->setMinimumSize(200,100);

    this->setMaximumSize(200,100);

    b1=new QPushButton("程序开始启动！",this);

    b1->setGeometry(20,20,160,40);

    QIcon icon = QIcon(":/img/gangtiexia.png");

    setWindowIcon(icon);

    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setIcon(icon);

    trayIcon->setToolTip("鼠标右击弹出菜单");

    QString titlec=tr("程序开始启动");

    QString textc=QString::fromLocal8Bit("Minimize to tray");

    trayIcon->show();

    trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);

    setWindowTitle(tr("Systray"));

    //托盘中鼠标右击的槽函数
    //    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
    //            this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(trayIcon,&QSystemTrayIcon::activated,this,&MyQWidget::iconActivated);
}
void MyQWidget::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
    case QSystemTrayIcon::Unknown:
        qDebug()<< "Unknown";
        break;
    case QSystemTrayIcon::Context:
        qDebug()<< "Context";
        break;
    case QSystemTrayIcon::Trigger:
        qDebug()<< "单击";
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug()<< "shuang击";
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug()<< "中间滚轮键点击击";
        break;
    default:
        ;
    }
}
