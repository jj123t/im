//
// Created by 25421 on 2024/5/1.
//

#include "client.h"
#include "msgmanager.h"
#include <common/async/asiotimer.h>

const std::string IP = "127.0.0.1";
const int PORT = 9000;

Client::Client() {
    mainReactors = new EventLoop();
    std::function<bool(const boost::system::error_code&, AsioTimerPtr)> cb
            = std::bind(&Client::sendMsg, this,
                        std::placeholders::_1, std::placeholders::_2);
    timer = AsioTimer::create(mainReactors->getIoContext(), 5000, cb);
    threadPool = std::make_unique<ThreadPool>(2);
}

void Client::newConnection() {
    tcpReactors = new EventLoop();
    tcpManager = new AsioTcpManager(tcpReactors->getIoContext());
    std::function<bool(const boost::system::error_code&, const char*,
                       const size_t, AsioTcpPtr )> cb
            = std::bind(&Client::recvMsg, this,
                        std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3, std::placeholders::_4);
    tcpId = tcpManager->addTransmission(IP, PORT, cb);
    msgManager = new MsgManager(tcpManager->getClient(tcpId));
    std::function<void(int)> cbr = std::bind(&Client::setUserId, this, std::placeholders::_1);
    msgManager->setUserId(cbr);
}

void Client::setUserId(int id) {
#ifdef DEBUG
    std::cout << "client set id: " << id << "\n";
#endif
    userid = id;
}

void Client::run() {
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, tcpReactors);
    threadPool->add(sub_loop);
    mainReactors->loop();
}

Client::~Client(){
    delete mainReactors;
    delete tcpReactors;
    delete tcpManager;
    delete msgManager;
}

bool Client::sendMsg(const boost::system::error_code& ec, AsioTimerPtr asioTimerPtr) {
    im::protocol::Message message;
    msgManager->sendHeartBeat(message);
    msgManager->send(message);
    return true;
}

bool Client::recvMsg(const boost::system::error_code &ec, const char * buf, const size_t len, AsioTcpPtr ptr) {
#ifdef DEBUG
    std::cout << "recv msg!!!\n";
#endif
    if (len == 0) {
        tcpManager->removeConnection(tcpId);
        std::function<bool(const boost::system::error_code&, const char*,
                           const size_t, AsioTcpPtr )> cb
                = std::bind(&Client::recvMsg, this,
                            std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3, std::placeholders::_4);
        tcpId = tcpManager->addTransmission(IP, PORT, cb);
        msgManager->setTcp(tcpManager->getClient(tcpId));
    }
    else {
        msgManager->paser(buf, len);
    }
    return true;
}

void Client::deleteConnection(AsioTcpManager* nb){
    delete nb;
}
