//
// Created by 25421 on 2024/5/1.
//

#include "server.h"
#include "msgmanager.h"
#include <common/async/asiotimer.h>

Server::Server() {
    mainReactors = new EventLoop();
    std::function<bool(const boost::system::error_code&, AsioTimerPtr)> cb
            = std::bind(&Server::sendMsg, this,
                        std::placeholders::_1, std::placeholders::_2);
    timer = AsioTimer::create(mainReactors->getIoContext(), 5000, cb);
    threadPool = std::make_unique<ThreadPool>(2);
}

void Server::newConnection() {
    tcpReactors = new EventLoop();
    tcpManager = new AsioTcpManager(tcpReactors->getIoContext());
    std::function<bool(const boost::system::error_code&, const char*,
                       const size_t, AsioTcpPtr )> cb
            = std::bind(&Server::recvMsg, this,
                        std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3, std::placeholders::_4);
    tcpId = tcpManager->addTransmission("127.0.0.1", 9000, cb);
    msgManager = new MsgManager(tcpManager->getClient(tcpId));
}

void Server::run() {
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, tcpReactors);
    threadPool->add(sub_loop);
    mainReactors->loop();
}

Server::~Server(){
}

bool Server::sendMsg(const boost::system::error_code& ec, AsioTimerPtr asioTimerPtr) {
//    tcpManager->removeConnection(tcpId);
//    std::function<bool(const boost::system::error_code&, const char*,
//                       const size_t, AsioTcpPtr )> cb
//            = std::bind(&Client::recvMsg, this,
//                        std::placeholders::_1, std::placeholders::_2,
//                        std::placeholders::_3, std::placeholders::_4);
//    tcpId = tcpManager->addTransmission("127.0.0.1", 9000, cb);
//    std::cout << tcpId << "\n";
//    msgManager->setTcp(tcpManager->getClient(tcpId));
    im::protocol::Message message;
    msgManager->sendHeartBeat(message);
    msgManager->send(message);
    std::cout << "send msg!!!\n";
    return true;
}

bool Server::recvMsg(const boost::system::error_code &ec, const char * buf, const size_t len, AsioTcpPtr ptr) {
    std::cout << "recv msg!!!\n";
    if (len == 0) {
        tcpManager->removeConnection(tcpId);
        std::function<bool(const boost::system::error_code&, const char*,
                           const size_t, AsioTcpPtr )> cb
                = std::bind(&Server::recvMsg, this,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3, std::placeholders::_4);
        tcpId = tcpManager->addTransmission("127.0.0.1", 9000, cb);
        msgManager->setTcp(tcpManager->getClient(tcpId));
    }
    else {
        msgManager->paser(buf, len);
    }
    return true;
}

void Server::deleteConnection(AsioTcpManager* nb){

    delete nb;
}
