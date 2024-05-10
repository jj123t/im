//
// Created by 25421 on 2024/5/1.
//

#ifndef TESTMYSQL_MSGMANAGER_H
#define TESTMYSQL_MSGMANAGER_H

#include <thirdparty/protobuf/msg.pb.h>
#include <thirdparty/protobuf/ack.pb.h>
#include <common/network/networkbase.h>

#define BUFFSIZE 1024

class MsgManager {
public:
    MsgManager(AsioTcpPtr tcpPtr);
    ~MsgManager();
    void paser(const char* buf, int len);
    AsioTcpPtr getTcp();
    void setTcp(AsioTcpPtr tcpPtr);
    void send(im::protocol::Message &msg);
    void setTime(im::protocol::Message &msg);
    void sendURegister(im::protocol::Message &msg, const std::string& username, const std::string& password, const std::string& introduction);
    void sendGRegister(im::protocol::Message &msg, int lordId, const std::string& groupname , const std::string& introduction);
    void sendLogin(im::protocol::Message &msg, int id, std::string password);
    void sendAFriendF1(im::protocol::Message &msg, int senderId, int receiverId, const std::string& message, int time);
    void sendAFriendF2(im::protocol::Message &msg, int senderId, int receiverId, bool status, const std::string& message, int time);
    void sendAGroupF1(im::protocol::Message &msg, int senderId, int groupId, const std::string& message, int time);
    void sendAGroupF2(im::protocol::Message &msg, int senderId, int groupId, bool status, const std::string& message, int time);
    void sendUChat(im::protocol::Message &msg, int senderId, int receiverId, const std::string& message, int time);
    void sendGChat(im::protocol::Message &msg, int senderId, int groupId, const std::string& message, int time) ;
    void sendSUser(im::protocol::Message &msg, int sendId, const std::string& message) ;
    void sendSGroup(im::protocol::Message &msg, int senderId, const std::string& message) ;
    void sendHeartBeat(im::protocol::Message &msg) ;
    void setUserId(std::function<void(int)>);
private:
    int userId;
    AsioTcpPtr asioTcpPtr;
    std::function<void(int)> callback;
};


#endif //TESTMYSQL_MSGMANAGER_H
