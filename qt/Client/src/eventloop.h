//
// Created by 25421 on 2024/5/2.
//

#ifndef CLIENT_EVENTLOOP_H
#define CLIENT_EVENTLOOP_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    boost::asio::io_context& getIoContext();
private:
    boost::asio::io_context ioContext;
    bool quit;
};


#endif //CLIENT_EVENTLOOP_H
