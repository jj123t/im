//
// Created by 25421 on 2024/4/25.
//

#include "acceptor.h"
#include "channel.h"

Acceptor::Acceptor(EventLoop* _loop) : loop(_loop), networkBase(nullptr), acceptChannel(nullptr) {
    networkBase = new NetworkBase("127.0.0.1", 9000, NetworkType::TCPSERVER);
    networkBase->listen();
    acceptChannel = new Channel( loop, networkBase->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
}

Acceptor::~Acceptor() {
}

void Acceptor::acceptConnection() {
    int fd = networkBase->accept();
    NetworkBase* newNB = new NetworkBase(fd, NetworkType::TCPSERVER);
    networkBase->setNonBlock(true);
    newConnectionCallback(newNB);
}

void Acceptor::setNewConnectionCallback(std::function<void(NetworkBase*)> _cb) {
    newConnectionCallback = _cb;
}