
#include <tool.h>


//数据转换参考 https://blog.csdn.net/biersibao/article/details/82884719
QString Tool::tenString2HexStr(QString message){
    QString hexString="";
    //"240,121,247"   ----->  [f0,79,f7]
    QStringList lis = message.split(',');
    for(int i = 0;i < lis.length();i++){
        int intt = lis[i].toUInt();
        QByteArray a = QByteArray::number(intt, 16);
        //f0
        hexString += a;
    }
    // f079f7
    return hexString;
}
