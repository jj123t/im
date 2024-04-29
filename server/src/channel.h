//
// Created by 25421 on 2024/4/26.
//
#pragma once

#ifndef SERVER_CHANNEL_H
#define SERVER_CHANNEL_H

#include <memory>
#include <functional>
#include <csignal>
#include <sys/epoll.h>

class EventLoop;
class Channel {
public:
    Channel(EventLoop* _loop, int _fd);
    ~Channel();
    void handleEvent();
    void enableRead();
    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    void useET();
    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
private:
    EventLoop* loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};
#endif //SERVER_CHANNEL_H
