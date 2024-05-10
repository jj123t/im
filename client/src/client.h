//
// Created by 25421 on 2024/5/1.
//

#ifndef TESTMYSQL_CLIENT_H
#define TESTMYSQL_CLIENT_H

#include <common/network/networkbase.h>
#include <common/threading/threadpool.h>
#include <common/async/asiotimer.h>
#include <src/msgmanager.h>
#include <src/eventloop.h>
#include <set>

class Client {
public:
    Client();
    ~Client();
    bool sendMsg(const boost::system::error_code&, AsioTimerPtr);
    bool recvMsg(const boost::system::error_code&, const char*, const size_t, AsioTcpPtr);
    void newConnection();
    void run();
    void setUserId(int id);
private:
    void deleteConnection(AsioTcpManager*);
    EventLoop* mainReactors;
    EventLoop* tcpReactors;
    std::unique_ptr<ThreadPool> threadPool;
    MsgManager* msgManager;
    AsioTcpManager* tcpManager;
    int tcpId;
    AsioTimerPtr timer;
    std::set<int> stTcpId;
    int userid;
    std::function<void()> callback;
};


#endif //TESTMYSQL_CLIENT_H
