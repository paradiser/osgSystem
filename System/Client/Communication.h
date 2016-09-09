#ifndef COMMUNITION_H
#define COMMUNITION_H

#include <QWidget>
#include <QStringList>
#include <QString>
#include <QProcess>
namespace Ui {
    class Communication;
}

class Communication : public QWidget
{
    Q_OBJECT

public:
    explicit Communication(QWidget *parent = 0);
    ~Communication();
    int writeScpFile(QStringList filenames);
    int writeRanderFile();

private:
    QProcess *rander_process;
    QProcess *upload_process;
    Ui::Communication *ui;
    int progressbar_flag;

private slots:
    void on_uploadFile_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_randerButton_clicked();
    void printOutput_rander();
    void printOutput_upload();
    void upload_process_started();
    void rander_process_finished(int , QProcess::ExitStatus);
    void upload_process_finished(int , QProcess::ExitStatus);
};

#endif // COMMUNITION_H
