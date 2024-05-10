//
// Created by 25421 on 2024/5/1.
//

#include <src/client.h>
#include <iostream>
#include <common/async/asiotimer.h>
#include <common/network/networkbase.h>

bool f(const boost::system::error_code &ec, const char * buf, const size_t len, AsioTcpPtr ptr) {
    std::cout << "recv msg!!!\n";
    return true;
}

int main() {
    EventLoop* ev = new EventLoop();
    std::unique_ptr<ThreadPool> threadPool = std::make_unique<ThreadPool>(1);
    std::function<bool(const boost::system::error_code&, const char*,
                       const size_t, AsioTcpPtr )> cb
            = std::bind(&f,
                        std::placeholders::_1, std::placeholders::_2,
                        std::placeholders::_3, std::placeholders::_4);
    AsioTcpManager* manager = new AsioTcpManager(ev->getIoContext());
    manager->addTransmission("127.0.0.1", 9000, cb);
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, ev);
    threadPool->add(sub_loop);
    while (true) {
        std::cout << "hello\n";
        sleep(2);
    }

    return 0;
}