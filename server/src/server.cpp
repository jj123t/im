//
// Created by 25421 on 2024/4/25.
//

#include "server.h"
#include "eventloop.h"

Server::Server(EventLoop* _loop): mainReactor(_loop) {
    acceptor = std::make_unique<Acceptor>(mainReactor);
    std::function<void(NetworkBase*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency();
    threadPool = std::make_unique<ThreadPool>(size);
    for(int i = 0; i < size; ++i){
        subReactors.push_back(new EventLoop());
    }
    for (int i = 0; i < size; i++) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        threadPool->add(sub_loop);
    }
}

void Server::newConnection(NetworkBase* nb) {
    if(nb->getFd() != -1){
        int random = nb->getFd() % subReactors.size();
        MsgManager* conn = new MsgManager(subReactors[random], nb);
        std::function<void(NetworkBase*)> callback = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(callback);
        managers[nb->getFd()] = conn;
    }
}

Server::~Server(){
    managers.clear();
    subReactors.clear();
}

void Server::deleteConnection(NetworkBase* nb){
    std::cout << nb->getFd() << " deleteConnection\n";
    if(nb->getFd() != -1){
        auto it = managers.find(nb->getFd());
        if(it != managers.end()){
            managers.erase(nb->getFd());
            delete nb;
        }
    }
}
