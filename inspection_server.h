#ifndef __INSPECTION_SERVER_HPP__
#define __INSPECTION_SERVER_HPP__

#include <QWebSocketServer>
#include <QWebSocket>
#include <QObject>
#include <iostream>
#include <memory>

class InspectionServer;

typedef std::shared_ptr<QWebSocketServer> QWebSocketServerPtr;
typedef std::shared_ptr<QWebSocket> QWebSocketPtr;
typedef std::shared_ptr<InspectionServer> InspectionServerPtr;

class InspectionServer: public QObject
{
    Q_OBJECT

    QWebSocketServerPtr websocketServer;
    QList<QWebSocketPtr> clients;

public:
    InspectionServer(uint16_t port);

signals:
    void closed();

private slots:
    void onNewConnection();
    void processTextMessage(const QString& message);
    void socketDisconnected();

};

#endif
