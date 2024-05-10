#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
    , registerForm(new Register())
    , login(new Login())
    , chatForm(new MainWindow())
{
    ui->setupUi(this);
    connect(login, &Login::signal_showLauncher, this, &Client::showLauncher);
    connect(login, &Login::signal_registerRequested, this, &Client::showRegisterForm);
    connect(registerForm, &Register::signal_registrationCompleted, this, &Client::hideRegisterFormAndShowLogin);
    hideRegisterFormAndShowLogin();
}

Client::~Client()
{
    delete ui;
    delete login;
    delete registerForm;
}
