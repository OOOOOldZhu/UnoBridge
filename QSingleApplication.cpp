#include <QLocalSocket>
#include <QLocalServer>
#include <QFile>
#include <QTextStream>
#include "QSingleApplication.h"
QSingleApplication::QSingleApplication(int argc, char **argv)
    : QApplication(argc, argv),
      m_bRunning(false)
{
    QCoreApplication::setOrganizationName("Company");
    QCoreApplication::setApplicationName("AppName");
    QString strServerName = QCoreApplication::organizationName() + QCoreApplication::applicationName();
    QLocalSocket socket;
    socket.connectToServer(strServerName);
    if (socket.waitForConnected(500))
    {
        QTextStream stream(&socket);
        QStringList args = QCoreApplication::arguments();
        QString strArg = (args.count() > 1) ? args.last() : "";
        stream << strArg;
        stream.flush();
        qDebug() << "Have already connected to server.";
        socket.waitForBytesWritten();
        m_bRunning = true;
    }
    else
    {
        // 如果不能连接到服务器，则创建一个
        m_pServer = new QLocalServer(this);
        connect(m_pServer, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));
        if (m_pServer->listen(strServerName))
        {
            // 防止程序崩溃，残留进程服务，直接移除
            if ((m_pServer->serverError() == QAbstractSocket::AddressInUseError) && QFile::exists(m_pServer->serverName()))
            {
                QFile::remove(m_pServer->serverName());
                m_pServer->listen(strServerName);
            }
        }
    }
}
void QSingleApplication::newLocalConnection()
{
    QLocalSocket *pSocket = m_pServer->nextPendingConnection();
    if (pSocket != NULL)
    {
        pSocket->waitForReadyRead(1000);
        QTextStream in(pSocket);
        QString strValue;
        in >> strValue;
        qDebug() << QString("The value is: %1").arg(strValue);
        delete pSocket;
        pSocket = NULL;
    }
}
bool QSingleApplication::isRunning()
{
    return m_bRunning;
}
