//
// Created by 25421 on 2024/5/1.
//

#ifndef TESTMYSQL_MSGMANAGER_H
#define TESTMYSQL_MSGMANAGER_H

#include "../thirdparty/protobuf/msg.pb.h"
#include "../thirdparty/protobuf/ack.pb.h"
#include "../common/network/networkbase.h"

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
    void sendLoginRequest(im::protocol::Message &msg, int id, std::string password);
    void sendAddFriend(im::protocol::Message &msg, int senderId, int receiverId, const std::string& message);
    void sendAddGroup(im::protocol::Message &msg, int senderId, int groupId, const std::string& message);
    void sendSingleChat(im::protocol::Message &msg, int senderId, int receiverId, const std::string& content);
    void sendGroupChat(im::protocol::Message &msg, int senderId, int groupId, const std::string& content) ;
    void sendSearchUser(im::protocol::Message &msg, int sendId, const std::string& message) ;
    void sendSearchGroup(im::protocol::Message &msg, int senderId, const std::string& message) ;
    void sendFileTransfer(im::protocol::Message &msg, int senderId, int receiverId) ;
    void sendHeartBeat(im::protocol::Message &msg) ;
private:
    AsioTcpPtr asioTcpPtr;
};


#endif //TESTMYSQL_MSGMANAGER_H
