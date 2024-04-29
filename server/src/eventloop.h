//
// Created by 25421 on 2024/4/25.
//

#ifndef SERVER_EVENTLOOP_H
#define SERVER_EVENTLOOP_H

#include <functional>
#include <memory>

class Channel;
class Epoller;
class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel*);
private:
    std::shared_ptr<Epoller> ep;
    bool quit;
};

#endif //SERVER_EVENTLOOP_H
