#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "src/login.h"
#include "src/mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Client;
}
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();

private slots:
    void showRegisterForm() {
        this->hide();
        chatForm->hide();
        login->hide();
        registerForm->resize(500, 500);
        registerForm->show();
    }
    void hideRegisterFormAndShowLogin() {
        this->hide();
        chatForm->hide();
        registerForm->hide();
        chatForm->hide();
        login->resize(500, 500);
        login->show();
    }
    void showLauncher() {
        this->hide();
        registerForm->hide();
        login->hide();
        chatForm->resize(500, 500);
        chatForm->show();
    }
private:
    Ui::Client *ui;
    Login *login;
    Register *registerForm;
    MainWindow *chatForm;
};
#endif // CLIENT_H
