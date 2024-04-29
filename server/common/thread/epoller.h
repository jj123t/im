//
// Created by 25421 on 2024/4/24.
//

#ifndef IM_EPOLLER_H
#define IM_EPOLLER_H

#include <functional>
#include <map>
#include <csignal>
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <common/util/util.h>
#include <src/channel.h>

#define LOG_INFO std::cout
#define LOG_ERROR std::cout
#define LOG_ENDL std::endl

class Epoller {
public:
    Epoller();
    ~Epoller();
    void updateChannel(Channel* );
    void deleteChannel(Channel* );
    std::vector<Channel*> poll(int timeout = -1);
private:
    static constexpr int MAX_EVENTS = 1000;
    int epfd;
    struct epoll_event* events;
};
#endif //IM_EPOLLER_H
