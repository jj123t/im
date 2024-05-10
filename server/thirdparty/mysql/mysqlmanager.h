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
        int id;
        std::string username;
        std::string introduction;
    };
    struct GroupInfo {
        int id;
        int lordId;
        std::string groupname;
        std::string introduction;
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
    struct FriendRecord {
        int senderId;
        int receiverId;
        std::string content;
        std::time_t time;
    };
    struct GroupRecord {
        int senderId;
        int groupId;
        std::string content;
        std::time_t time;
    };
    static void init(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
    static bool connect();

    // 用户注册返回id
    static int userRegister(const std::string& username, const std::string& password, const std::string& introduction = "");
    // 用户登陆返回状态
    static int userLogin(int userId, const std::string& password);
    static int setUserHB(int userId, int time);
    static int getUserHB(int userId);
    // 删除个人信息
    static int delUser(int userId);
    // 更新个人信息
    static int updUser(int userId, const std::string& username, const std::string& password, const std::string& introduction = "");
    // 获取个人信息
    static UserInfo getUserInfo(int userId);

    // 群注册返回群id
    static int groupRegister(int lord_id, const std::string& groupname, const std::string& introduction = "");
    // 删除群
    static int delGroup(int groupId);
    // 更新群信息
    static int updGroup(int lordId, int groupId, const std::string& groupname, const std::string& introduction);
    // 获取群信息
    static GroupInfo getGroupInfo(int groupId);

    // 添加 删除 好友
    static int addFriend(int senderId, int receiverId);
    static int delFriend(int senderId, int receiverId);
    // 保存 获取添加好友请求
    static int postFriend(int senderId, int receiverId, const std::string& message, int time);
    static std::vector<FriendRecord> getFriend(int senderId, int time);
    // 获取好友列表
    static std::vector<UserInfo> getFriends(int id);
    // 加入 退出群组
    static int addGroup(int senderId, int groupId);
    static int delGroup(int senderId, int groupId);
    // 保存 获取添加群组请求
    static int postGroup(int senderId, int groupId, const std::string& message, int time);
    static std::vector<GroupRecord> getGroup(int senderId, int time);
    // 获取群组列表
    static std::vector<GroupInfo> getGroupsL(int id);
    static std::vector<GroupInfo> getGroupsU(int id);
    // 获取群成员
    static std::vector<UserInfo> getGroupMember(int id);
    // 获取群主
    static int getLord(int groupId);
    // 用户聊天记录
    static int addUserChat(int senderId, int receiverId, const std::string& content, std::time_t time);
    static int delUserChat(int chatId);
    static std::vector<UserChatRecord> getUserChat(int senderId, int receiverId, time_t startTimestamp, time_t endTimestamp);
    static std::vector<UserChatRecord> getUserChatOne(int receiverId, time_t startTimestamp, time_t endTimestamp);
    // 群组聊天记录
    static int addGroupChat(int senderId, int groupId, const std::string& content, std::time_t time);
    static int delGroupChat(int chatId);
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
