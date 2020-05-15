#ifndef TOBOARDCOMM_H
#define TOBOARDCOMM_H

#include <QByteArray>
#include <QObject>

class IToBoardComm: public QObject
{

public:
    virtual void init() = 0;
    virtual void send(QByteArray arr) = 0;
    virtual void closeComm() = 0;



};



#endif // TOBOARDCOMM_H
