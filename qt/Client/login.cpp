#include "login.h"
#include "ui_login.h"
#include <QApplication>

Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_login_button_clicked()
{
    emit signal_showLauncher();
}


void Login::on_register_button_clicked()
{
    emit signal_registerRequested();
}

