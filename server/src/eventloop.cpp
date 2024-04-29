//
// Created by 25421 on 2024/4/25.
//

#include "eventloop.h"
#include <common/thread/epoller.h>
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = std::make_shared<Epoller>();
}

EventLoop::~EventLoop() {
}

void EventLoop::loop() {
    while (!quit) {
        std::vector<Channel*> chs;
        chs = ep->poll();
        for (auto it = chs.begin(); it != chs.end(); it++) {
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch) {
    ep->updateChannel(ch);
}