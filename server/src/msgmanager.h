//
// Created by 25421 on 2024/4/24.
//

#ifndef SERVER_MSGMANAGER_H
#define SERVER_MSGMANAGER_H

#include <common/network/networkbase.h>
#include <common/async/buffer.h>
#include <memory>
#include <functional>
#include <map>
//#include "../thirdparty/protobuf/msg.pb.h"
#include <thirdparty/protobuf/msg.pb.h>
#include <thirdparty/protobuf/ack.pb.h>
#include <thirdparty/mysql/mysqlmanager.h>

#define SIZE 1024

class EventLoop;
class Channel;
class MsgManager {
public:
    void setDeleteConnectionCallback(std::function<void(NetworkBase*)>);
    MsgManager(EventLoop* _loop, NetworkBase* _sock);
    ~MsgManager();
    void handleURegister(im::protocol::Message &msg);
    void handleGRegister(im::protocol::Message &msg);
    void handleLogin(im::protocol::Message &msg);
    void handleUChat(im::protocol::Message &msg);
    void handleGChat(im::protocol::Message &msg);
    void handleAFriendFrom1(im::protocol::Message &msg);
    void handleAFriendFrom2(im::protocol::Message &msg);
    void handleAGroupFrom1(im::protocol::Message &msg);
    void handleAGroupFrom2(im::protocol::Message &msg);
    void handleSearchUser(im::protocol::Message &msg);
    void handleSearchGroup(im::protocol::Message &msg);
    void handleHeartBeat(im::protocol::Message &msg);

    void setTime(im::protocol::Ack &msg);
    void getTime(im::protocol::Message &msg);

    void sendAckURegister(int id, int userId);
    void sendAckGRegister(int id, int groupId);
    void sendAckLogin(int id, bool status);
    void sendAckUChat(int id, int senderId, int receiverId, const std::string& message, int time);
    void sendAckGChat(int id, int senderId, int groupId, const std::string& message, int time);
    void sendAckUserInfo(int id, int userId, const std::string& username, const std::string& introduction);
    void sendAckGroupInfo(int id, int groupId, int lordId, const std::string& groupname, const std::string& introduction);
    void sendAckAFT1(int id, int senderId, int receiverId, const std::string& message, bool status, int time);
    void sendAckAFT2(int id, int senderId, int receiverId, const std::string& message, int time);
    void sendAckAGT1(int id, int senderId, int groupId, int lordId, const std::string& message, bool status, int time);
    void sendAckAGT2(int id, int senderId, int groupId, int lordId, const std::string& message, int time);
    void sendAckSUser(int id, std::vector<MysqlManager::UserInfo> t);
    void sendAckSGroup(int id, std::vector<MysqlManager::GroupInfo> t);
    void sendAckSGMember(int id, std::vector<MysqlManager::UserInfo> t);
    void paser(NetworkBase*);
private:
    EventLoop* loop;
    std::map<int, NetworkBase*> nb;
    NetworkBase* networkBase;
    NetworkBase** tonb;
    Channel* channel;
    std::function<void(NetworkBase*)> deleteConnectionCallback;
    std::shared_ptr<Buffer> readBuffer;
};


#endif //SERVER_MSGMANAGER_H
