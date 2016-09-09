#include "mainInterface.h"
#include "ui_mainInterface.h"
#include <QDebug>
mainInterface::mainInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainInterface)
{
    ui->setupUi(this);
    a = 0;
}

mainInterface::~mainInterface()
{
    delete ui;
}

void mainInterface::on_pushButton_clicked()
{
    /*QFile fp("../shell/scp_file.ep");
    if(fp.open(QFile::WriteOnly)) {
        QTextStream out(&fp);
        out << "#!/usr/bin/expect" << endl;
        out << "set timeout 6" << endl;
        out << "spawn scp -o ConnectTimeout=5 " << serverName << "@" << serverIp << endl;
        out << "expect {" << endl << "\t" << "\"no)?\" {" << endl;
        out << "\t\t" << "send \"yes\\r\"" << endl;
        out << "\t\t" << "expect \":\"" << endl;
        out << "\t\t" << "send \"" << serverPwd << "\\r\"" << endl << "\t}" << endl;
        out << "\t" << "\":\" {" << endl << "\t\t" << "send \"" << serverPwd
            << "\\r\"" << endl << "\t}" << endl;
        out << "}" << endl << "interact" << endl;
    }*/
    accept();
}

void mainInterface::on_pushButton_2_clicked() {
    close();
}
