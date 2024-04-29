//
// Created by 25421 on 2024/4/24.
//


#include <src/eventloop.h>
#include <src/server.h>
#include <src/eventloop.h>
#include <common/network/mysqlmanager.h>

int main() {
    std::string host = "127.0.0.1";
    std::string user = "root";
    std::string password = "hec93448";
    std::string database = "server";
    MysqlManager::init(host, user, password, database);
    EventLoop* loop = new EventLoop();
    std::shared_ptr<Server> server = std::make_shared<Server>(loop);
    loop->loop();
    return 0;
}