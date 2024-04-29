//
// Created by 25421 on 2024/4/25.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <common/thread/threadpool.h>
#include <memory>
#include <functional>
#include <vector>
#include <unistd.h>
#include <src/msgmanager.h>
#include <src/acceptor.h>

class Server {
public:
    Server(EventLoop*);
    ~Server();
    void loop();
private:
    void newConnection(NetworkBase*);
    void deleteConnection(NetworkBase*);
    std::unique_ptr<Acceptor> acceptor;
    EventLoop* mainReactor;
    std::map<int, MsgManager*> managers;
    std::vector<EventLoop*> subReactors;
    std::unique_ptr<ThreadPool> threadPool;
};


#endif //SERVER_SERVER_H
