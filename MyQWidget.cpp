#include "MyQWidget.h"
#include<QTextCodec>
#include<QIcon>
#include<QPushButton>
#include<QTextCodec>
#include<QTextCodec>
#include<QTextCodec>
#include<QTextCodec>
#include<QTextCodec>



MyQWidget::MyQWidget()

{

        //QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));

        this->setMinimumSize(200,100);

        this->setMaximumSize(200,100);

        b1=new QPushButton("hello world",this);

        b1->setGeometry(20,20,160,40);

        QIcon icon = QIcon(":/img/gangtiexia.ico");

        setWindowIcon(icon);

        trayIcon = new QSystemTrayIcon(this);

        trayIcon->setIcon(icon);

        trayIcon->setToolTip("a trayicon example");

        QString titlec=tr("Hello");

        QString textc=QString::fromLocal8Bit("Hello QT?");

        trayIcon->show();

        trayIcon->showMessage(titlec,textc,QSystemTrayIcon::Information,5000);

        setWindowTitle(tr("Systray"));

}
