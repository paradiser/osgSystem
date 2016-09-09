#include "Communication.h"
#include "ui_Communication.h"
#include "mainInterface.h"
#include <QDialog>
#include <QFile>
#include <QDebug>
#include <QTextEdit>
#include <QFileDialog>
#include <QProgressDialog>
#include <QProgressBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QScrollBar>
#include <unistd.h>

Communication::Communication(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Communication)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    progressbar_flag = 1;

    rander_process = new QProcess(this);
    upload_process = new QProcess(this);


    connect(upload_process , SIGNAL(started()) , this ,
            SLOT(upload_process_started()));
    connect(rander_process , SIGNAL(readyReadStandardOutput()) , this ,
            SLOT(printOutput_rander()));
    connect(upload_process , SIGNAL(readyReadStandardOutput()) , this ,
            SLOT(printOutput_upload()));
    connect(rander_process , SIGNAL(finished(int,QProcess::ExitStatus)) , this ,
            SLOT(rander_process_finished(int,QProcess::ExitStatus)));
    connect(upload_process , SIGNAL(finished(int,QProcess::ExitStatus)) , this ,
            SLOT(upload_process_finished(int,QProcess::ExitStatus)));


    QString log = tr("服务器ip为") + "127.0.0.1\n";
    //ui->label->setText(server_ip);
    ui->outputDialog->setReadOnly(true);
    ui->outputDialog->setText(log);
}

Communication::~Communication()
{
    delete ui;
}

int Communication::writeScpFile(QStringList filenames) {
    //输入服务器用户名，ip，密码
    bool ok;
    // 获取服务器信息
    QString serverIp = QInputDialog::getText(this,tr("服务器ip"),
              tr("请输入服务器ip："),QLineEdit::Normal,tr(""),&ok);
    if(!ok) return 0;
    QString serverName = QInputDialog::getText(this,tr("服务器用户名"),
              tr("请输入服务器用户名："),QLineEdit::Normal,tr("root"),&ok);
    if(!ok) return 0;
    QString serverPwd = QInputDialog::getText(this,tr("服务器密码"),
              tr("请输入服务器密码："),QLineEdit::Normal,tr(""),&ok);
    if(!ok) return 0;
    QString serverPath = "/home/paradiser/desktop/Test";
    for(int i=0; i<filenames.size(); i++) {
        QFile fp("../shell/scp_file.ep");
        if(fp.open(QFile::WriteOnly)) {
            QTextStream out(&fp);
            out << "#!/usr/bin/expect" << endl;
            out << "set timeout 6" << endl;
            out << "spawn scp -o ConnectTimeout=5 " << filenames[i] << " " << serverName << "@" << serverIp
                << ":" << serverPath << endl;
            out << "expect {" << endl << "\t" << "\"no)?\" {" << endl;
            out << "\t\t" << "send \"yes\\r\"" << endl;
            out << "\t\t" << "expect \"word:\"" << endl;
            out << "\t\t" << "send \"" << serverPwd << "\\r\"" << endl << "\t}" << endl;
            out << "\t" << "\":\" {" << endl << "\t\t" << "send \"" << serverPwd
                << "\\r\"" << endl << "\t}" << endl;
            out << "}" << endl << "interact" << endl;
        }
        fp.close();
    }
    return 1;
}

// 选择文件
void Communication::on_uploadFile_clicked()
{
    int flag = 1;
    // 提示对话框
    QMessageBox::information(this,tr("注意"),
                        tr("请上传可执行文件和对应的数据文件！（可分开上传）"),QMessageBox::Ok);
    QFileDialog *file = new QFileDialog();
    QStringList filenames = file->getOpenFileNames(this , tr("选择文件") , "/home");
    qDebug() << "filenames: " << filenames;

    if(filenames.empty() == 0) {
        //写scp_file脚本并执行
        bool ret = writeScpFile(filenames);
        if(!ret) flag = 0;
    } else {
        flag = 0;
    }
    if(flag) {
        QString command = "sh ../shell/scp";
        upload_process->start(command);
    } else {
        ui->progressBar->setVisible(false);
    }
}

// 重新登录按钮
void Communication::on_pushButton_clicked()
{
    // 先关闭主界面，其实是隐藏起来了，并没有真正退出
    close();
    // 新建mainInterface对象
    mainInterface newInterface;

    if(newInterface.exec()== QDialog::Accepted) {
        QString server_ip = tr("服务器ip为") + "127.0.0.1\n";
        ui->label->setText(server_ip);
        ui->outputDialog->setText("");
        progressbar_flag = 1;
        show();
    }
}

void Communication::on_pushButton_2_clicked(){
    close();
}
int Communication::writeRanderFile() {
    //输入服务器用户名，ip，密码
    bool ok;
    // 获取服务器信息
    QString serverIp = QInputDialog::getText(this,tr("服务器ip"),
              tr("请输入服务器ip："),QLineEdit::Normal,tr(""),&ok);
    if(!ok) return 0;
    QString serverName = QInputDialog::getText(this,tr("服务器用户名"),
              tr("请输入服务器用户名："),QLineEdit::Normal,tr("root"),&ok);
    if(!ok) return 0;
    QString serverPwd = QInputDialog::getText(this,tr("服务器密码"),
              tr("请输入服务器密码："),QLineEdit::Normal,tr(""),&ok);
    if(!ok) return 0;

    QFile fp("../shell/make_docker.ep");
    if(fp.open(QFile::WriteOnly)) {
        QTextStream out(&fp);
        out << "#!/usr/bin/expect" << endl;
        out << "set timeout 10" << endl;
        out << "spawn ssh -X " << serverName << "@" << serverIp << endl;
        out << "expect {" << endl << "\t" << "\"no)?\" {" << endl;
        out << "\t\t" << "send \"yes\\r\"" << endl;
        out << "\t\t" << "expect \"word:\"" << endl;
        out << "\t\t" << "send \"" << serverPwd << "\\r\"" << endl << "\t}" << endl;
        out << "\t" << "\":\" {" << endl << "\t\t" << "send \"" << serverPwd
            << "\\r\"" << endl << "\t}" << endl;
        out << "}" << endl << endl;

        out << "expect \"$\"" << endl;
        out << "send \"sudo nvidia-docker run --rm -ti -v /etc/localtime:/etc/localtime:ro --net=host -e DISPLAY=:10.0 -v /home/" << serverName
            << "/.Xauthority:/root/.Xauthority --name abc nvidia/cuda:osg\\r\"" << endl;
        out << "expect \":\"" << endl;
        out << "send \"" << serverPwd << "\\r\"" << endl;
        out << "expect \"#\"" << endl;
        out << "send \"" << "exit" << "\\r\"" << endl;
        out << "expect \"#\"" << endl;
        out << "send \"" << "osgviewer cow.osg" << "\\r\"" << endl;
        out << "expect \"#\"" << endl;
        out << "send \"" << "exit" << "\\r\"" << endl;
        out << "expect \"$\"" << endl;
        out << "send \"" << "exit" << "\\r\"" << endl;
        out << "interact" << endl;
    }
    fp.close();

    return 1;
}

//渲染按钮
void Communication::on_randerButton_clicked() {
    if(!writeRanderFile()) {
        ui->outputDialog->append("write rander file failed.");
        return;
    }
    QString command = "sh ../shell/rander";
    rander_process->start(command);
    if(rander_process->waitForStarted() == false) {
        ui->outputDialog->append("the process cannot be called.");
    }
}

void Communication::printOutput_rander() {
    QString log = rander_process->readAll();
    ui->outputDialog->append(log);
    //滚动条滑块置底
    ui->outputDialog->moveCursor(QTextCursor::End);
}

void Communication::printOutput_upload() {
    QString log = upload_process->readAll();
    qDebug() << log << endl;
    if(log.indexOf("lost connection") == -1) progressbar_flag = 0;
    ui->outputDialog->append(log);
    //滚动条滑块置底
    ui->outputDialog->moveCursor(QTextCursor::End);
}

void Communication::upload_process_started() {
    ui->outputDialog->append("upload_process started.");
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);
    /*// 进度对话框
    QProgressDialog dialog(tr("文件上传进度"),tr("取消"),0,100,this);
    // 设置窗口标题
    dialog.setWindowTitle(tr("上传文件中"));
    // 将对话框设置为模态
    dialog.setWindowModality(Qt::WindowModal);
    dialog.show();
    // 演示复制进度
    for(int i=0;i<=50000;i++){
        // 设置进度条的当前值
        dialog.setValue(i);
        // 避免界面冻结
        QCoreApplication::processEvents();
        // 按下取消按钮则中断
        if(dialog.wasCanceled()) {
            flag = 0;
            break;
        }
    }*/
}

void Communication::upload_process_finished(int exitCode, QProcess::ExitStatus exitStatus) {
    if(exitStatus == QProcess::NormalExit) {
        ui->outputDialog->append("upload_process exit normal.");
    } else {
        ui->outputDialog->append("upload_process exit normal.");
    }
    qDebug() << upload_process->readAllStandardOutput() << endl;
    ui->outputDialog->append("upload_process finished.");
    if(progressbar_flag)
        ui->progressBar->setValue(100);
    else ui->progressBar->setValue(0);
}

void Communication::rander_process_finished(int exitCode, QProcess::ExitStatus exitStatus) {
    if(exitStatus == QProcess::NormalExit) {
        ui->outputDialog->append("rander_process exit normal.");
    } else {
        ui->outputDialog->append("rander_process exit normal.");
    }
    ui->outputDialog->append("rander_process finished.");
}
