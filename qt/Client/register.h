#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

signals:
    void signal_registrationCompleted();

private slots:
    void on_send_register_button_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
