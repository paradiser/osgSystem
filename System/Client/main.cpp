#include <QApplication>
#include "Communication.h"
#include "mainInterface.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Communication w;
    mainInterface mInterface; //新建mainInterface类对象
    if(mInterface.exec() == QDialog::Accepted){ //判断mInterface执行结果
        w.show();         //如果是“连接服务器”，则进入通信界面
        return a.exec(); //程序正常运行
    }
    else return 0;   //否则，退出程序
}
