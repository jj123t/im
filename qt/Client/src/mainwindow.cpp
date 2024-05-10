
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString msg = "hello world!!!";
    qint64 unixTimestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    QString time = QString::number(unixTimestamp);
    QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    dealMessage(messageW, item, msg, time, QNChatMessage::User_She);

    resize(500, 500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString msg = ui->textEdit->toPlainText() + " ";
    ui->textEdit->setText("");
    qint64 unixTimestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    QString time = QString::number(unixTimestamp);
    // QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    bool isSending = true; // 发送中
    // bool isSending = false;

    qDebug()<<"addMessage" << msg << time << ui->listWidget->count();
    // if(ui->listWidget->count()%2) {
        if(isSending) {
            dealMessageTime(time);
            QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
        } else {
            bool isOver = true;
            for(int i = ui->listWidget->count() - 1; i > 0; i--) {
                QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
                if(messageW->text() == msg) {
                    isOver = false;
                    messageW->setTextSuccess();
                }
            }
            if(isOver) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
                dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
                messageW->setTextSuccess();
            }
        }
    // } else {
    //     if(msg != "") {
    //         dealMessageTime(time);

    //         QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
    //         QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

    //         // dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
    //         dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
    //     }
    // }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void MainWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    ui->textEdit->resize(this->width() - 20, ui->widget->height() - 20);
    ui->textEdit->move(10, 10);

    ui->pushButton->move(ui->textEdit->width()+ui->textEdit->x() - ui->pushButton->width() - 10,
                         ui->textEdit->height()+ui->textEdit->y() - ui->pushButton->height() - 10);


    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }
}
