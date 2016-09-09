#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QDialog>

namespace Ui {
    class mainInterface;
}

class mainInterface : public QDialog
{
    Q_OBJECT

public:
    explicit mainInterface(QWidget *parent = 0);
    ~mainInterface();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::mainInterface *ui;
    int a;
};

#endif // MAININTERFACE_H
