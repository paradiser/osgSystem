/********************************************************************************
** Form generated from reading UI file 'Communication.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMUNICATION_H
#define UI_COMMUNICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Communication
{
public:
    QPushButton *uploadFile;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *randerButton;
    QTextEdit *outputDialog;
    QProgressBar *progressBar;

    void setupUi(QWidget *Communication)
    {
        if (Communication->objectName().isEmpty())
            Communication->setObjectName(QStringLiteral("Communication"));
        Communication->resize(400, 300);
        uploadFile = new QPushButton(Communication);
        uploadFile->setObjectName(QStringLiteral("uploadFile"));
        uploadFile->setGeometry(QRect(30, 30, 75, 23));
        label = new QLabel(Communication);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 30, 141, 16));
        pushButton = new QPushButton(Communication);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 260, 75, 23));
        pushButton_2 = new QPushButton(Communication);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(290, 260, 75, 23));
        randerButton = new QPushButton(Communication);
        randerButton->setObjectName(QStringLiteral("randerButton"));
        randerButton->setGeometry(QRect(280, 30, 75, 23));
        outputDialog = new QTextEdit(Communication);
        outputDialog->setObjectName(QStringLiteral("outputDialog"));
        outputDialog->setGeometry(QRect(20, 60, 341, 191));
        progressBar = new QProgressBar(Communication);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(120, 30, 118, 23));
        progressBar->setValue(24);

        retranslateUi(Communication);

        QMetaObject::connectSlotsByName(Communication);
    } // setupUi

    void retranslateUi(QWidget *Communication)
    {
        Communication->setWindowTitle(QApplication::translate("Communication", "Communication", 0));
        uploadFile->setText(QApplication::translate("Communication", "\344\270\212\344\274\240\346\226\207\344\273\266", 0));
        label->setText(QApplication::translate("Communication", "<html><head/><body><p><br/></p></body></html>", 0));
        pushButton->setText(QApplication::translate("Communication", "\351\207\215\346\226\260\350\277\236\346\216\245", 0));
        pushButton_2->setText(QApplication::translate("Communication", "\351\200\200\345\207\272", 0));
        randerButton->setText(QApplication::translate("Communication", "rander", 0));
    } // retranslateUi

};

namespace Ui {
    class Communication: public Ui_Communication {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMUNICATION_H
