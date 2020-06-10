#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H
#include <QApplication>
class QLocalServer;
class QSingleApplication : public QApplication
{
    Q_OBJECT
public:
    explicit QSingleApplication(int argc, char **argv);
    // 判断进程是否存在
    bool isRunning();
private slots:
    void newLocalConnection();
private:
    QLocalServer *m_pServer;
    bool m_bRunning;
};
#endif // SINGLE_APPLICATION_H
