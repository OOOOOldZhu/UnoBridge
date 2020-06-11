#ifndef MYCLASS_H
#define MYCLASS_H

#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include <QSystemTrayIcon>

class MyQWidget: public QWidget

{

public:

        MyQWidget();

private:

        QPushButton *b1;

        QSystemTrayIcon *trayIcon;

        QMenu *trayIconMenu;

        void iconActivated(QSystemTrayIcon::ActivationReason reason);

        void generateMenu();
};





#endif // MYCLASS_H
