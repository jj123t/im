//
// Created by 25421 on 2024/4/25.
//

#ifndef SERVER_ACCEPTOR_H
#define SERVER_ACCEPTOR_H


#include <functional>
#include <common/network/networkbase.h>
#include <memory>

class EventLoop;
class Channel;
class Acceptor {
public:
    Acceptor(EventLoop* _loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(NetworkBase*)>);
private:
    EventLoop* loop;
    NetworkBase* networkBase;
    Channel* acceptChannel;
    std::function<void(NetworkBase*)> newConnectionCallback;
};


#endif //SERVER_ACCEPTOR_H
