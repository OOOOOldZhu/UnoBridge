#ifndef TOOL_H
#define TOOL_H
#include <QObject>

class Tool;

class Tool:public QObject{

public:
    static QString tenString2HexStr(QString message);
    static QByteArray tenString2ByteArray(QString message);
    static QByteArray tenString2ByteArray2(QString message);
    static QString ByteArrayToHexString(QByteArray data);



};

#endif // TOOL_H
