#ifndef TOOL_H
#define TOOL_H
#include <QObject>

class Tool;

class Tool:public QObject{

public:
    static QString tenString2HexStr(QString message);



};












#endif // TOOL_H
