//
// Created by 25421 on 2024/4/24.
//

#include "epoller.h"


Epoller::Epoller() : epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new struct epoll_event[MAX_EVENTS];
    ::bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoller::~Epoller(){
    delete events;
}

std::vector<Channel*> Epoller::poll(int timeout){
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setReady(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoller::updateChannel(Channel* channel){
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()){
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
        channel->setInEpoll();
    } else{
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
    }
}

void Epoller::deleteChannel(Channel* channel){
    int fd = channel->getFd();
    errif(epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1, "epoll delete error");
    channel->setInEpoll(false);
}