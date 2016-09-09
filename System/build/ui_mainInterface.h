/********************************************************************************
** Form generated from reading UI file 'mainInterface.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAININTERFACE_H
#define UI_MAININTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_mainInterface
{
public:
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;

    void setupUi(QDialog *mainInterface)
    {
        if (mainInterface->objectName().isEmpty())
            mainInterface->setObjectName(QStringLiteral("mainInterface"));
        mainInterface->resize(400, 300);
        pushButton = new QPushButton(mainInterface);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(60, 170, 75, 23));
        pushButton_2 = new QPushButton(mainInterface);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 170, 75, 23));
        label = new QLabel(mainInterface);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(160, 60, 51, 21));

        retranslateUi(mainInterface);

        QMetaObject::connectSlotsByName(mainInterface);
    } // setupUi

    void retranslateUi(QDialog *mainInterface)
    {
        mainInterface->setWindowTitle(QApplication::translate("mainInterface", "mainInterface", 0));
        pushButton->setText(QApplication::translate("mainInterface", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", 0));
        pushButton_2->setText(QApplication::translate("mainInterface", "\351\200\200\345\207\272\347\250\213\345\272\217", 0));
        label->setText(QApplication::translate("mainInterface", "\346\254\242\350\277\216!", 0));
    } // retranslateUi

};

namespace Ui {
    class mainInterface: public Ui_mainInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAININTERFACE_H
