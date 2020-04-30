
#include <tool.h>
#include <QDebug>

//数据转换参考 https://blog.csdn.net/biersibao/article/details/82884719
QString Tool::tenString2HexStr(QString message){
    QString hexString="";
    //"240,121,247"   ----->  [f0,79,f7]
    QStringList lis = message.split(',');
    for(int i = 0;i < lis.length();i++){
        int intt = lis[i].toUInt();
        QByteArray a = QByteArray::number(intt, 16);
        //f0
        hexString = hexString+a+" ";
    }
    // f079f7
    return hexString;
}

QByteArray Tool::tenString2ByteArray(QString tenStr){
    //第一步，tenStr去除空格
    tenStr = tenStr.trimmed();
    tenStr = tenStr.simplified();
    QByteArray byteArray;
    //第二步,  "240,121,247" -----> "f0 79 f7"
    QStringList lis = tenStr.split(',');
    bool ok;
    for(int i = 0;i < lis.length();i++){
        int intt = lis[i].toUInt();
        QByteArray a = QByteArray::number(intt, 16);
        //f0
        char c = a.toInt(&ok,16) & 0xff;
        if(ok){
            byteArray.append(c);
        }else{
            qDebug()<<"非法的16进制字符："<<a;
        }
    }

    //qDebug()<<"转换完毕之后: "<<byteArray;
    return byteArray;
}

QByteArray Tool::tenString2ByteArray2(QString tenStr){
    //第一步，tenStr去除空格
    tenStr = tenStr.trimmed();
    tenStr = tenStr.simplified();

    //第二步,  "240,121,247" -----> "f0 79 f7"
    QString hexStr="";
    QStringList lis = tenStr.split(',');
    bool ok;
    for(int i = 0;i < lis.length();i++){
        int intt = lis[i].toUInt();
        QByteArray a = QByteArray::number(intt, 16);
        hexStr = hexStr + a + " ";
    }
    //第三步
    QByteArray ret;
    QStringList sl = hexStr.split(" ");
    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toUInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{
                qDebug()<<"非法的16进制字符："<<s;
            }
        }
    }

    return ret;
}

QString Tool::ByteArrayToHexString(QByteArray data){
    QString ret(data.toHex().toUpper());
    int len = ret.length()/2;
    //qDebug()<<len;
    for(int i=1;i<len;i++)
    {
        //qDebug()<<i;
        ret.insert(2*i+i-1," ");
    }

    return ret;
}
