//
// Created by 25421 on 2024/5/1.
//

#ifndef TESTMYSQL_CLIENT_H
#define TESTMYSQL_CLIENT_H

#include "../common/network/networkbase.h"
#include "../common/threading/threadpool.h"
#include "../common/async/asiotimer.h"
#include "msgmanager.h"
#include "eventloop.h"
#include <set>

class Server {
public:
    Server();
    ~Server();
    bool sendMsg(const boost::system::error_code&, AsioTimerPtr);
    bool recvMsg(const boost::system::error_code&, const char*, const size_t, AsioTcpPtr);
    void newConnection();
    void run();
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
};


#endif //TESTMYSQL_CLIENT_H
