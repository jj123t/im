//
// Created by 25421 on 2024/5/4.
//

#include <src/client.h>
#include <iostream>
#include <common/async/asiotimer.h>
#include <common/network/networkbase.h>

MsgManager* res;

bool f(const boost::system::error_code &ec, const char * buf, const size_t len, AsioTcpPtr ptr) {
    res->paser(buf, len);
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
    int tid = manager->addTransmission("127.0.0.1", 9000, cb);
    res = new MsgManager(manager->getClient(tid));
    std::function<void()> sub_loop = std::bind(&EventLoop::loop, ev);
    threadPool->add(sub_loop);
    while (true) {
        int type;
        std::cin >> type;
        im::protocol::Message msg;
        if (type == 1) {
            res->sendHeartBeat(msg);
        }
        else if (type == 2) {
            std::string username, password, introduction;
            std::cin >> username >> password >> introduction;
            res->sendURegister(msg, username, password, introduction);
        }
        else if (type == 3) {
            int id;
            std::string groupname, introduction;
            std::cin >> id >> groupname >> introduction;
            res->sendGRegister(msg, id, groupname, introduction);
        }
        else if (type == 4) {
            int id;
            std::string password;
            std::cin >> id >> password;
            res->sendLogin(msg, id, password);
        }
        else if (type == 5) {
            int s, r;
            std::string mes;
            std::cin >> s >> r >> mes;
            std::time_t t = std::time(0);
            res->sendUChat(msg, s, r, mes, t);
        }
        else if (type == 6) {
            int s, g;
            std::string mes;
            std::cin >> s >> g >> mes;
            std::time_t t = std::time(0);
            res->sendGChat(msg, s, g, mes, t);
        }
        else if (type == 7) {
            int s, r;
            std::string mes;
            std::cin >> s >> r >> mes;
            std::time_t t = std::time(0);
            res->sendAFriendF1(msg, s, r, mes, t);
        }
        else if (type == 8) {
            int s, r, status;
            std::string mes;
            std::cin >> s >> r >> status >> mes;
            std::time_t t = std::time(0);
            res->sendAFriendF2(msg, s, r, status, mes, t);
        }
        else if (type == 9) {
            int s, r;
            std::string mes;
            std::cin >> s >> r >> mes;
            std::time_t t = std::time(0);
            res->sendAGroupF1(msg, s, r, mes, t);
        }
        else if (type == 10) {
            int s, r, status;
            std::string mes;
            std::cin >> s >> r >> status >> mes;
            std::time_t t = std::time(0);
            res->sendAGroupF2(msg, s, r, status, mes, t);
        }
        res->send(msg);
    }
    return 0;
}