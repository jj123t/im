/********************************************************************************
** Form generated from reading UI file 'register.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTER_H
#define UI_REGISTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *repeat_label;
    QLabel *username_label;
    QLabel *once_password_label;
    QLineEdit *username_edit;
    QLineEdit *once_password_edit;
    QLineEdit *repeat_password_edit;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *send_register_button;

    void setupUi(QWidget *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName("Register");
        Register->resize(400, 300);
        gridLayoutWidget = new QWidget(Register);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(30, 40, 160, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        repeat_label = new QLabel(gridLayoutWidget);
        repeat_label->setObjectName("repeat_label");

        gridLayout->addWidget(repeat_label, 2, 0, 1, 1);

        username_label = new QLabel(gridLayoutWidget);
        username_label->setObjectName("username_label");

        gridLayout->addWidget(username_label, 0, 0, 1, 1);

        once_password_label = new QLabel(gridLayoutWidget);
        once_password_label->setObjectName("once_password_label");

        gridLayout->addWidget(once_password_label, 1, 0, 1, 1);

        username_edit = new QLineEdit(gridLayoutWidget);
        username_edit->setObjectName("username_edit");

        gridLayout->addWidget(username_edit, 0, 1, 1, 1);

        once_password_edit = new QLineEdit(gridLayoutWidget);
        once_password_edit->setObjectName("once_password_edit");

        gridLayout->addWidget(once_password_edit, 1, 1, 1, 1);

        repeat_password_edit = new QLineEdit(gridLayoutWidget);
        repeat_password_edit->setObjectName("repeat_password_edit");

        gridLayout->addWidget(repeat_password_edit, 2, 1, 1, 1);

        verticalLayoutWidget = new QWidget(Register);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(30, 140, 161, 31));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        send_register_button = new QPushButton(verticalLayoutWidget);
        send_register_button->setObjectName("send_register_button");

        verticalLayout->addWidget(send_register_button);


        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QWidget *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "Form", nullptr));
        repeat_label->setText(QCoreApplication::translate("Register", "repeat:", nullptr));
        username_label->setText(QCoreApplication::translate("Register", "username:", nullptr));
        once_password_label->setText(QCoreApplication::translate("Register", "password:", nullptr));
        send_register_button->setText(QCoreApplication::translate("Register", "register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTER_H
