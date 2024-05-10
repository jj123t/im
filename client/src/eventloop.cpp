//
// Created by 25421 on 2024/5/2.
//

#include "eventloop.h"

EventLoop::EventLoop() {

}
EventLoop::~EventLoop() {

}
boost::asio::io_context& EventLoop::getIoContext() {
    return ioContext;
}

void EventLoop::loop() {
    ioContext.run();
}

