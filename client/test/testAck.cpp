//
// Created by 25421 on 2024/5/1.
//

#include <src/client.h>
#include <iostream>
#include <common/async/asiotimer.h>
#include <common/network/networkbase.h>
#include <src/msgmanager.h>
#include <thread>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

MsgManager* msgManager;

bool recvMsg(const boost::system::error_code &ec, const char * buf, const size_t len, AsioTcpPtr ptr) {
    std::cout << "client: " << buf << "\n";
//    msgManager->paser(buf, len);
    im::protocol::Ack abc;
    abc.ParseFromArray(buf, sizeof(buf));
    if (abc.type() == im::protocol::Ack_Type::Ack_Type_LOGIN_REQUEST) {
        std::cout << "YEs\n";
    }
    else {
        std::cout << "No\n";
    }
//    msgManager->paser(buf, len);
    return true;
}

void f1() {
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(9000);  //端口
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 20);
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    while(1)
    {
        char buf[1024];
        im::protocol::Ack msg;
        msg.set_type(im::protocol::Ack_Type::Ack_Type_LOGIN_REQUEST);
        auto* loginRequest = msg.mutable_login_request();
        loginRequest->set_status(1);
        msg.SerializeToArray(buf,BUFFSIZE);
        write(clnt_sock, buf, sizeof(buf));
        std::cout << "server: " << buf << "\n";
        sleep(5);
    }
    close(clnt_sock);
    close(serv_sock);
}

void f2() {
    boost::asio::io_context io;
    AsioTcpManager* manager = new AsioTcpManager(io);
    std::function<bool(const boost::system::error_code&, const char*,
                       const size_t, AsioTcpPtr )> cb
            = std::bind(&recvMsg,
                        std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3, std::placeholders::_4);
    int id = manager->addTransmission("127.0.0.1", 9000, cb);
    im::protocol::Message msg;
    msg.set_type(im::protocol::Message::HEARTBEAT);
    char buff[BUFFSIZE];
    msg.SerializeToArray(buff,BUFFSIZE);
    std::cout << "server: " << buff << "\n";
    manager->getClient(id)->send(buff, sizeof(buff));
    msgManager = new MsgManager(manager->getClient(id));
    io.run();
}

int main() {
//    Client* client = new Client();
//    client->newConnection();
//    client->run();

    std::thread t1(f1);
    std::thread t2(f2);
    t1.join();
    t2.join();
    return 0;
}