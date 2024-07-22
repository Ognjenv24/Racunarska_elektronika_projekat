/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLineEdit *lineEditTemperature;
    QLineEdit *lineEditFanSpeed;
    QPushButton *buttonIncreaseTemp;
    QPushButton *buttonDecreaseTemp;
    QPushButton *buttonFanLow;
    QPushButton *buttonFanMedium;
    QPushButton *buttonFanHigh;
    QLabel *label;
    QLabel *label_2;
    QLabel *labelACStatus;
    QPushButton *buttonToggleAC;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(800, 600);
        lineEditTemperature = new QLineEdit(Dialog);
        lineEditTemperature->setObjectName(QString::fromUtf8("lineEditTemperature"));
        lineEditTemperature->setGeometry(QRect(160, 100, 113, 30));
        lineEditTemperature->setReadOnly(true);
        lineEditFanSpeed = new QLineEdit(Dialog);
        lineEditFanSpeed->setObjectName(QString::fromUtf8("lineEditFanSpeed"));
        lineEditFanSpeed->setGeometry(QRect(310, 100, 113, 30));
        lineEditFanSpeed->setReadOnly(true);
        buttonIncreaseTemp = new QPushButton(Dialog);
        buttonIncreaseTemp->setObjectName(QString::fromUtf8("buttonIncreaseTemp"));
        buttonIncreaseTemp->setGeometry(QRect(160, 220, 91, 30));
        buttonDecreaseTemp = new QPushButton(Dialog);
        buttonDecreaseTemp->setObjectName(QString::fromUtf8("buttonDecreaseTemp"));
        buttonDecreaseTemp->setGeometry(QRect(160, 260, 91, 30));
        buttonFanLow = new QPushButton(Dialog);
        buttonFanLow->setObjectName(QString::fromUtf8("buttonFanLow"));
        buttonFanLow->setGeometry(QRect(380, 220, 91, 30));
        buttonFanMedium = new QPushButton(Dialog);
        buttonFanMedium->setObjectName(QString::fromUtf8("buttonFanMedium"));
        buttonFanMedium->setGeometry(QRect(500, 220, 91, 30));
        buttonFanHigh = new QPushButton(Dialog);
        buttonFanHigh->setObjectName(QString::fromUtf8("buttonFanHigh"));
        buttonFanHigh->setGeometry(QRect(610, 220, 91, 30));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(160, 70, 121, 22));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(340, 70, 68, 22));
        labelACStatus = new QLabel(Dialog);
        labelACStatus->setObjectName(QString::fromUtf8("labelACStatus"));
        labelACStatus->setGeometry(QRect(490, 350, 68, 22));
        buttonToggleAC = new QPushButton(Dialog);
        buttonToggleAC->setObjectName(QString::fromUtf8("buttonToggleAC"));
        buttonToggleAC->setGeometry(QRect(140, 360, 91, 30));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        buttonIncreaseTemp->setText(QCoreApplication::translate("Dialog", "plus", nullptr));
        buttonDecreaseTemp->setText(QCoreApplication::translate("Dialog", "minus", nullptr));
        buttonFanLow->setText(QCoreApplication::translate("Dialog", "Low", nullptr));
        buttonFanMedium->setText(QCoreApplication::translate("Dialog", "Med", nullptr));
        buttonFanHigh->setText(QCoreApplication::translate("Dialog", "High", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Trenutna Temp", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Brzina", nullptr));
        labelACStatus->setText(QCoreApplication::translate("Dialog", "TextLabel", nullptr));
        buttonToggleAC->setText(QCoreApplication::translate("Dialog", "on/off", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
