#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "register.h"
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

signals:
    void signal_registerRequested();
    void signal_showLauncher();

private slots:
    void on_login_button_clicked();
    void on_register_button_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
