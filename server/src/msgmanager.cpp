//
// Created by 25421 on 2024/4/24.
//

#include "msgmanager.h"
#include "channel.h"
#include <sstream>
#include <iomanip>
#include "thirdparty/mysql/mysqlmanager.h"

MsgManager::MsgManager(EventLoop* _loop, NetworkBase* _sock) : loop(_loop), networkBase(_sock), channel(nullptr), readBuffer(nullptr) {
    channel = new Channel(loop, networkBase->getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> cb = std::bind(&MsgManager::paser, this, _sock);
    channel->setReadCallback(cb);
    readBuffer = std::shared_ptr<Buffer>();
}
void MsgManager::setTime(im::protocol::Ack &msg) {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    msg.mutable_timestamp()->set_seconds(secs);
}
void MsgManager::getTime(im::protocol::Message &msg) {
    std::time_t tt = msg.timestamp().seconds();
    std::tm * local_time = std::localtime(&tt);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
#ifdef DEBUG
    std::cout << "Timestamp as string: " << oss.str() << std::endl;
#endif
}
void MsgManager::sendAckURegister(int id, int userId) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKUREGISTER);
    auto* loginRequest = msg.mutable_uregister();
    loginRequest->set_id(userId);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckGRegister(int id, int groupId) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKGREGISTER);
    auto* gregister = msg.mutable_gregister();
    gregister->set_id(groupId);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckLogin(int id, bool status) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKLOGIN);
    auto* login = msg.mutable_login();
    login->set_id(id);
    login->set_status(status);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
//    networkBase->send(ackBuff, sizeof(ackBuff));
    (*tonb)->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckUChat(int id, int senderId, int receiverId, const std::string& message, int time) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKUCHAT);
    auto* uchat = msg.mutable_uchat();
    uchat->set_sender_id(senderId);
    uchat->set_receiver_id(receiverId);
    uchat->set_message(message);
    uchat->set_time(time);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckGChat(int id, int senderId, int groupId, const std::string& message, int time) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKGCHAT);
    auto* gchat = msg.mutable_gchat();
    gchat->set_sender_id(senderId);
    gchat->set_group_id(groupId);
    gchat->set_message(message);
    gchat->set_time(time);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckUserInfo(int id, int userId, const std::string& username, const std::string& introduction) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKUINFO);
    auto* uinfo = msg.mutable_uinfo();
    uinfo->set_id(userId);
    uinfo->set_username(username);
    uinfo->set_introduction(introduction);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
//    networkBase->send(ackBuff, sizeof(ackBuff));
    (*tonb)->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckGroupInfo(int id, int groupId, int lordId, const std::string& groupname, const std::string& introduction) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKGINFO);
    auto* ginfo = msg.mutable_ginfo();
    ginfo->set_id(groupId);
    ginfo->set_lord_id(lordId);
    ginfo->set_groupname(groupname);
    ginfo->set_introduction(introduction);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
//    networkBase->send(ackBuff, sizeof(ackBuff));
    (*tonb)->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckAFT1(int id, int senderId, int receiverId, const std::string& message, bool status, int time) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKAFT1);
    auto* aft1 = msg.mutable_aft1();
    aft1->set_sender_id(senderId);
    aft1->set_receiver_id(receiverId);
    aft1->set_message(message);
    aft1->set_status(status);
    aft1->set_time(time);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckAFT2(int id, int senderId, int receiverId, const std::string& message, int time) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKAFT1);
    auto* aft2 = msg.mutable_aft2();
    aft2->set_sender_id(senderId);
    aft2->set_receiver_id(receiverId);
    aft2->set_message(message);
    aft2->set_time(time);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckAGT1(int id, int senderId, int groupId, int lordId, const std::string& message, bool status, int time) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKAGT1);
    auto* agt1 = msg.mutable_agt1();
    agt1->set_sender_id(senderId);
    agt1->set_group_id(groupId);
    agt1->set_lord_id(lordId);
    agt1->set_message(message);
    agt1->set_status(status);
    agt1->set_time(time);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckAGT2(int id, int senderId, int groupId, int lordId, const std::string& message, int time) {
    im::protocol::Ack msg;
    setTime(msg);
    msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKAGT2);
    auto* agt2 = msg.mutable_agt2();
    agt2->set_sender_id(senderId);
    agt2->set_group_id(groupId);
    agt2->set_lord_id(lordId);
    agt2->set_message(message);
    agt2->set_time(time);
    char ackBuff[SIZE];
    msg.SerializeToArray(ackBuff,sizeof(ackBuff));
    networkBase->send(ackBuff, sizeof(ackBuff));
}
void MsgManager::sendAckSUser(int id, std::vector<MysqlManager::UserInfo> t) {
    for (auto x : t) {
        im::protocol::Ack msg;
        setTime(msg);
        msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKSUSER);
        auto *suser = msg.mutable_suser();
        suser->set_id(x.id);
        suser->set_username(x.username);
        suser->set_introduction(x.introduction);
        char ackBuff[SIZE];
        msg.SerializeToArray(ackBuff, sizeof(ackBuff));
        networkBase->send(ackBuff, sizeof(ackBuff));
    }
}
void MsgManager::sendAckSGroup(int id, std::vector<MysqlManager::GroupInfo> t) {
    for (auto x : t) {
        im::protocol::Ack msg;
        setTime(msg);
        msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKSUSER);
        auto *suser = msg.mutable_suser();
        suser->set_id(x.id);
        suser->set_username(x.groupname);
        suser->set_introduction(x.introduction);
        char ackBuff[SIZE];
        msg.SerializeToArray(ackBuff, sizeof(ackBuff));
        networkBase->send(ackBuff, sizeof(ackBuff));
    }
}
void MsgManager::sendAckSGMember(int id, std::vector<MysqlManager::UserInfo> t) {
    for (auto x : t) {
        im::protocol::Ack msg;
        setTime(msg);
        msg.set_type(im::protocol::Ack_Type::Ack_Type_ACKSGMember);
        auto *suser = msg.mutable_sgmember();
        suser->set_id(x.id);
        suser->set_username(x.username);
        suser->set_introduction(x.introduction);
        char ackBuff[SIZE];
        msg.SerializeToArray(ackBuff, sizeof(ackBuff));
        networkBase->send(ackBuff, sizeof(ackBuff));
    }
}

void MsgManager::handleURegister(im::protocol::Message &msg) {
    auto* uregister = msg.mutable_uregister();
    std::string username = uregister->username();
    std::string password = uregister->password();
    std::string introduction = uregister->introduction();
    if (username == "" or password == "" or introduction == "") return ;
    int id = MysqlManager::userRegister(username, password, introduction);
    sendAckURegister(id, id);
#ifdef DEBUG
    std::cout << "this is a uregister message !!!\n";
    std::cout << "userId: " << id << "\n";
    std::cout << "username: " << username << "\n";
    std::cout << "password: " << password << "\n";
#endif
}
void MsgManager::handleGRegister(im::protocol::Message &msg) {
    auto* gregister = msg.mutable_gregister();
    int lordId = gregister->lord_id();
    std::string groupname = gregister->groupname();
    std::string introduction = gregister->introduction();
    if (lordId == 0 or groupname == "" or introduction == "") return ;
    int id = MysqlManager::groupRegister(lordId, groupname, introduction);
    sendAckGRegister(lordId, id);
#ifdef DEBUG
    std::cout << "this is a gregister message !!!\n";
    std::cout << "lord id is: " << lordId << "\n";
    std::cout << "groupname: " << groupname << "\n";
    std::cout << "group id is: " << id << "\n";
    std::cout << "introduction: " << introduction << "\n";
#endif
}
void MsgManager::handleLogin(im::protocol::Message &msg) {
    auto* login = msg.mutable_login();
    int userId = login->user_id();
    std::string password = login->password();
    bool status = MysqlManager::userLogin(userId, password);
    sendAckLogin(userId, status);
    //
    auto users = MysqlManager::getFriends(userId);
    for (auto uu : users) {
        sendAckUserInfo(userId, uu.id, uu.username, uu.introduction);
    }
    //
    auto groups = MysqlManager::getGroupMember(userId);
    for (auto gg : groups) {
        sendAckGroupInfo(userId, gg.id, userId, gg.username, gg.introduction);
    }
#ifdef DEBUG
    std::cout << "this is a login message !!!\n";
    std::cout << "userId: " << userId << "\n";
    std::cout << " password: " << password << "\n";
    std::cout << "status: " << status << "\n";
#endif
}
void MsgManager::handleUChat(im::protocol::Message &msg) {
    auto* single_chat = msg.mutable_uchat();
    int sendId = single_chat->sender_id();
    int recvId = single_chat->receiver_id();
    std::string content = single_chat->message();
    int time = single_chat->time();
    MysqlManager::addUserChat(sendId, recvId, content, time);
    sendAckUChat(recvId, sendId, recvId, content, time);
#ifdef DEBUG
    std::cout << "this is a userChat message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << "recvid: " << recvId << "\n";
    std::cout << "text: " << content << "\n";
    std::cout << "time: " << time << "\n";
#endif
}
void MsgManager::handleGChat(im::protocol::Message &msg) {
    auto* groupChat = msg.mutable_gchat();
    int sendId = groupChat->sender_id();
    int groupId = groupChat->group_id();
    std::string content = groupChat->message();
    int time = groupChat->time();
    MysqlManager::addGroupChat(sendId, groupId, content, time);
    sendAckUChat(MysqlManager::getLord(groupId), sendId, groupId, content, time);
#ifdef DEBUG
    std::cout << "this is a group message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << "groupid: " << groupId << "\n";
    std::cout << "text: " << content << "\n";
    std::cout << "time: " << time << "\n";
#endif
}
void MsgManager::handleAFriendFrom1(im::protocol::Message &msg) {
    auto* add_friend1 = msg.mutable_aff1();
    int sendId = add_friend1->sender_id();
    int recvId = add_friend1->receiver_id();
    std::string message = add_friend1->message();
    int time = add_friend1->time();
    MysqlManager::postFriend(sendId, recvId, message, time);
    sendAckAFT2(recvId, sendId, recvId, message, time);
#ifdef DEBUG
    std::cout << "this is a addfriend1 message !!!\n";
    std::cout << "sendid is: " << sendId << "\n";
    std::cout << " recvid is: " << recvId << "\n";
    std::cout << " text is: " << message << "\n";
#endif
}
void MsgManager::handleAFriendFrom2(im::protocol::Message &msg) {
    auto* add_friend2 = msg.mutable_aff2();
    int sendId = add_friend2->sender_id();
    int recvId = add_friend2->receiver_id();
    bool status = add_friend2->status();
    std::string message = add_friend2->message();
    int time = add_friend2->time();
    if (status) {
        MysqlManager::addFriend(sendId, recvId);
    }
    sendAckAFT2(recvId, sendId, recvId, message, time);
#ifdef DEBUG
    std::cout << "this is a addfriend2 message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << " recvid: " << recvId << "\n";
    std::cout << " status: " << status << "\n";
#endif
}
void MsgManager::handleAGroupFrom1(im::protocol::Message &msg) {
    auto* add_group1 = msg.mutable_agf1();
    int sendId = add_group1->sender_id();
    int groupId = add_group1->group_id();
    std::string message = add_group1->message();
    int time = add_group1->time();
    MysqlManager::postGroup(sendId, groupId, message, time);
    sendAckAGT2(MysqlManager::getLord(groupId), sendId, groupId, MysqlManager::getLord(groupId), message, time);
#ifdef DEBUG
    std::cout << "this is a addfriend1 message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << "groupId: " << groupId << "\n";
    std::cout << "text: " << message << "\n";
    std::cout << "time: " << time << "\n";
#endif
}
void MsgManager::handleAGroupFrom2(im::protocol::Message &msg) {
    auto* add_group2 = msg.mutable_agf2();
    int sendId = add_group2->sender_id();
    int groupId = add_group2->group_id();
    bool status = add_group2->status();
    std::string message = add_group2->message();
    int time = add_group2->time();
    if (status) {
        MysqlManager::addGroup(sendId, groupId);
    }
    sendAckAGT1(sendId, sendId, groupId, MysqlManager::getLord(groupId), message, status, time);
#ifdef DEBUG
    std::cout << "this is a addfriend2 message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << "groupId: " << groupId << "\n";
    std::cout << "status: " << status << "\n";
#endif
}
void MsgManager::handleSearchUser(im::protocol::Message &msg) {
    auto* searchUser = msg.mutable_suser();
    int sendId = searchUser->sender_id();
    std::string text = searchUser->message();
#ifdef DEBUG
    std::cout << "this is a search user message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << " text is: " << text << "\n";
#endif
}
void MsgManager::handleSearchGroup(im::protocol::Message &msg) {
    auto* searchGroup = msg.mutable_sgroup();
    int sendId = searchGroup->sender_id();
    std::string text = searchGroup->message();
#ifdef DEBUG
    std::cout << "this is a search group message !!!\n";
    std::cout << "sendid: " << sendId << "\n";
    std::cout << "text: " << text << "\n";
#endif
}
void MsgManager::handleHeartBeat(im::protocol::Message &msg) {
    auto* heartbeat = msg.mutable_heartbeat();
    int id = heartbeat->id();
    // 获取 UChat
    auto userchat = MysqlManager::getUserChatOne(id, MysqlManager::getUserLastLogin(id), std::time(0));
    for (auto uc : userchat) {
        sendAckUChat(id, uc.senderId, uc.receiverId, uc.content, uc.time);
    }
    // 获取好友
    auto friends = MysqlManager::getFriend(id, MysqlManager::getUserLastLogin(id));
    for (auto f : friends) {
        sleep(1);
        sendAckAFT2(id, f.senderId, f.receiverId, f.content, f.time);
    }

    auto groups = MysqlManager::getGroups(id);
    // 获取GChat
    for (auto g : groups) {
        auto groupchat = MysqlManager::getGroupChat(id, g.id, MysqlManager::getUserLastLogin(id), std::time(0));
        for (auto gc: groupchat) {
            sleep(1);
            sendAckGChat(id, gc.senderId, gc.groupId, gc.content, gc.time);
        }
    }
    // 获取群组
    for (auto g : groups) {
        auto gq = MysqlManager::getGroup(g.id, MysqlManager::getUserLastLogin(id));
        for (auto r : gq) {
            sendAckAGT2(id, r.senderId, r.groupId, id, r.content, r.time);
        }
    }
#ifdef DEBUG
    std::cout << "this is a heartbeat message !!!\n";
#endif
}

void MsgManager::paser(NetworkBase* nb) {
    networkBase = nb;
    tonb = &nb;
    // 读取到buff中，解析成Message
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int BUFSIZE = 1024;
    char buff[BUFSIZE];
    nb->recv(buff, sizeof(buff));
    im::protocol::Message received_proto_message;
    received_proto_message.ParseFromArray(buff, sizeof(buff));
    // 根据message的type处理不同消息
    if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_UREGISTER) {
        // 处理注册请求
        handleURegister(received_proto_message);
    }
    if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_GREGISTER) {
        // 注册群
        handleGRegister(received_proto_message);
    }
    if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_LOGIN) {
        // 处理登陆请求
        handleLogin(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_UCHAT) {
        // 处理发来的单聊
        handleUChat(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_GCHAT) {
        // 处理发来的群聊
        handleGChat(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_AFF1) {
        // 处理添加好友请求
        handleAFriendFrom1(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_AFF2) {
        // 处理返回的好友确认
        handleAFriendFrom2(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_AGF1) {
        handleAGroupFrom1(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_AGF2) {
        handleAGroupFrom2(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_SUSER) {
        // 处理搜索id请求
        handleSearchUser(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_SGROUP) {
        // 处理搜索群id请求
        handleSearchGroup(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_HEARTBEAT) {
        // 处理心跳
        handleHeartBeat(received_proto_message);
    }
    else {
        // 不存在的类型
#ifdef DEBUG
        std::cout << "error parser !!!\n";
#endif
    }
    // 断开连接
    //    deleteConnectionCallback(nb);
}

MsgManager::~MsgManager() {
}

void MsgManager::setDeleteConnectionCallback(std::function<void(NetworkBase*)> _cb){
    deleteConnectionCallback = _cb;
}