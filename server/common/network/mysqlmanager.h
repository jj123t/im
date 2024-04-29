//
// Created by 25421 on 2024/4/24.
//

#ifndef IM_MYSQLMANAGER_H
#define IM_MYSQLMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"
#include <ctime>


class MysqlManager {
public:
    struct UserInfo {
        int userId;
        std::string username;
        std::string password;
        std::string introduction;
    };
    struct GroupInfo {
        int groupId;
        int lordId;
        std::string groupname;
        std::string introduction;
    };
    struct FriendShipList {
        int userId;
        std::vector<UserInfo> frientList;
    };
    struct GroupList {
        int userId;
        std::vector<GroupInfo> groupList;
    };
    struct UserChatRecord {
        int senderId;
        int receiverId;
        std::string content;
        std::time_t time;
    };
    struct GroupChatRecord {
        int groupId;
        int senderId;
        std::string content;
        std::time_t time;
    };
    static void init(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
    static bool connect();
    // 添加 删除好友
    static int makeFriend(int senderId, int receiverId);
    static int tieFriend(int senderId, int receiverId);
    // 加入 退出群组
    static int makeGroup(int senderId, int groupId);
    static int tieGroup(int senderId, int groupId);
    // 用户   注册，登陆，删除，更新
    static int userRegister(const std::string& username, const std::string& password, const std::string& introduction = "");
    static int userLogin(int userId, const std::string& password);
    static int userDelete(int userId);
    static int userUpdate(int userId, const std::string& username, const std::string& password, const std::string& introduction = "");
    static UserInfo getUserInfo(int userId);
    // 群组   注册，登陆，删除，更新
    static int groupRegister(int lord_id, const std::string& groupname, const std::string& introduction = "");
    static int groupDelete(int groupId);
    static int groupUpdate(int groupId, const std::string& groupname, const std::string& introduction);
    static GroupInfo getGroupInfo(int groupId);
    // 用户聊天记录
    static int insertUserChat(int senderId, int receiverId, const std::string& content, std::time_t time);
    static std::vector<UserChatRecord> getUserChat(int senderId, int receiverId, time_t startTimestamp, time_t endTimestamp);
    // 群组聊天记录
    static int insertGroupChat(int senderId, int groupId, const std::string& content, std::time_t time);
    static std::vector<GroupChatRecord> getGroupChat(int senderId, int groupId, time_t startTimestamp, time_t endTimestamp);

private:
    static sql::mysql::MySQL_Driver* driver_;
    static sql::Connection* con_;
    static std::string host_;
    static std::string user_;
    static std::string password_;
    static std::string database_;
};


#endif //IM_MYSQLMANAGER_H
