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

class EventLoop;
class Channel;
class MsgManager {
public:
    void setDeleteConnectionCallback(std::function<void(NetworkBase*)>);
    MsgManager(EventLoop* _loop, NetworkBase* _sock);
    ~MsgManager();
    void handleLoginRequest(im::protocol::Message &msg);
    void handleSingleChat(im::protocol::Message &msg);
    void handleGroupChat(im::protocol::Message &msg);
    void handleFileTransfer(im::protocol::Message &msg);
    void handleAddFriend(im::protocol::Message &msg);
    void handleAddGroup(im::protocol::Message &msg);
    void handleHeartBeat(im::protocol::Message &msg);
    void handleSearchUser(im::protocol::Message &msg);
    void handleSearchGroup(im::protocol::Message &msg);
    void paser(NetworkBase*);
private:
    EventLoop* loop;
    NetworkBase* networkBase;
    Channel* channel;
    std::function<void(NetworkBase*)> deleteConnectionCallback;
    std::shared_ptr<Buffer> readBuffer;
};


#endif //SERVER_MSGMANAGER_H
