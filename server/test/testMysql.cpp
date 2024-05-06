//
// Created by 25421 on 2024/4/28.
//
#include "thirdparty/mysql/mysqlmanager.h"
#include <chrono>

int main() {
    std::string host = "127.0.0.1";
    std::string user = "root";
    std::string password = "hec93448";
    std::string database = "server";
    MysqlManager::init(host, user, password, database);
    if (MysqlManager::connect()) {
        std::cout << "yes\n";
    }
    else {
        std::cout << "no\n";
    }
    // test Register;
//    std::cout << MysqlManager::userRegister("abc", "hec93448", "hello i am user!!") << "\n";
//    std::cout << MysqlManager::userUpdate(2, "b", "hec93448", "hello i am user b") << "\n";
//    std::cout << MysqlManager::groupRegister(2, "group 2", "hello i am b group") << "\n";
//    // test Login;
//    std::cout << MysqlManager::userLogin(1, "hec93448") << "\n";
//    std::cout << MysqlManager::userLogin(1, "12345") << "\n";
//    // test addFirend;
//    std::cout << MysqlManager::makeFriend(1, 2) << "\n";
//    std::cout << MysqlManager::makeGroup(3, 1) << "\n";
//    // test addChat;
//    auto now = std::chrono::system_clock::now();
//    auto duration = now.time_since_epoch();
//    auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
//    std::cout << MysqlManager::insertUserChat(1, 2, "hello i am user chat 1 and 2", secs) << "\n";
//    std::cout << MysqlManager::insertGroupChat(4, 1, "hello i am group chat 1 and 2", secs) << "\n";
//    // test get info
    MysqlManager::UserInfo userinfo = MysqlManager::getUserInfo(1);
    MysqlManager::GroupInfo groupinfo  = MysqlManager::getGroupInfo(1);
//    //
    return 0;
}