/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *account_label;
    QLabel *password_label;
    QLineEdit *account_edit;
    QLineEdit *password_edit;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *forget_button;
    QPushButton *register_button;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *login_button;

    void setupUi(QWidget *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName("Login");
        Login->resize(247, 227);
        gridLayoutWidget = new QWidget(Login);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(30, 40, 160, 80));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        account_label = new QLabel(gridLayoutWidget);
        account_label->setObjectName("account_label");

        gridLayout->addWidget(account_label, 0, 0, 1, 1);

        password_label = new QLabel(gridLayoutWidget);
        password_label->setObjectName("password_label");

        gridLayout->addWidget(password_label, 1, 0, 1, 1);

        account_edit = new QLineEdit(gridLayoutWidget);
        account_edit->setObjectName("account_edit");

        gridLayout->addWidget(account_edit, 0, 1, 1, 1);

        password_edit = new QLineEdit(gridLayoutWidget);
        password_edit->setObjectName("password_edit");

        gridLayout->addWidget(password_edit, 1, 1, 1, 1);

        horizontalLayoutWidget = new QWidget(Login);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(30, 130, 161, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        forget_button = new QPushButton(horizontalLayoutWidget);
        forget_button->setObjectName("forget_button");

        horizontalLayout->addWidget(forget_button);

        register_button = new QPushButton(horizontalLayoutWidget);
        register_button->setObjectName("register_button");

        horizontalLayout->addWidget(register_button);

        verticalLayoutWidget = new QWidget(Login);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(30, 170, 161, 31));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        login_button = new QPushButton(verticalLayoutWidget);
        login_button->setObjectName("login_button");

        verticalLayout->addWidget(login_button);


        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QWidget *Login)
    {
        Login->setWindowTitle(QCoreApplication::translate("Login", "Form", nullptr));
        account_label->setText(QCoreApplication::translate("Login", "account:", nullptr));
        password_label->setText(QCoreApplication::translate("Login", "password:", nullptr));
        forget_button->setText(QCoreApplication::translate("Login", "forget", nullptr));
        register_button->setText(QCoreApplication::translate("Login", "register", nullptr));
        login_button->setText(QCoreApplication::translate("Login", "login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
