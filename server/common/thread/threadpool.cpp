//
// Created by 25421 on 2024/4/24.
//

#include <iostream>
#include "threadpool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
    for (int i = 0; i < size; i++) {
        threads.emplace_back(std::thread([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });
                    if (stop && tasks.empty()) {
                        return ;
                    }
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}
ThreadPool::~ThreadPool() {
    std::cout << "i am ~ThreadPoo\n";
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for (std::thread &th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }
}