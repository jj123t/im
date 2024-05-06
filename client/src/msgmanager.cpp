//
// Created by 25421 on 2024/5/1.
//

#include <iomanip>
#include "msgmanager.h"

MsgManager::MsgManager(AsioTcpPtr tcpPtr) {
    asioTcpPtr = tcpPtr;
}
MsgManager::~MsgManager() {
}
void MsgManager::setUserId(std::function<void(int)> cb) {
    callback = cb;
}
void MsgManager::paser(const char* buf, int len) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    im::protocol::Ack received_proto_message;
    received_proto_message.ParseFromArray(buf, 1024);
    std::time_t tt = received_proto_message.timestamp().seconds();
    std::tm *local_time = std::localtime(&tt);
    // 使用std::put_time格式化日期和时间
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
#ifdef DEBUG
    // 输出格式化后的时间字符串
    std::cout << "Timestamp as string: " << oss.str() << std::endl;
#endif
    received_proto_message.timestamp().seconds();
    if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKUREGISTER) {
        auto* uregister = received_proto_message.mutable_uregister();
        int id = uregister->id();
#ifdef DEBUG
        std::cout << "this is a uregister ack!!!\n";
        std::cout << "userId: " << id << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKGREGISTER) {
        auto* gregister = received_proto_message.mutable_gregister();
        int id = gregister->id();
#ifdef DEBUG
        std::cout << "this is a gregister ack!!!\n";
        std::cout << "groupId: " << id << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKLOGIN) {
        auto* login = received_proto_message.mutable_login();
        int status = login->status();
        int id = login->id();
        userId = id;
#ifdef DEBUG
//        callback(id);
        std::cout << "this is a login ack!!!\n";
        std::cout << "status: " << status << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKUCHAT) {
        auto* uchat = received_proto_message.mutable_uchat();
        int senderId = uchat->sender_id();
        int receiverId = uchat->receiver_id();
        std::string message = uchat->message();
        int time = uchat->time();
#ifdef DEBUG
        std::cout << "this is a uchat ack!!!\n";
        std::cout << "senderId: " << senderId << "\n";
        std::cout << "receiverId: " << receiverId << "\n";
        std::cout << "message: " << message << "\n";
        std::cout << "time: " << time << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKGCHAT) {
        auto* gchat = received_proto_message.mutable_gchat();
        int senderId = gchat->sender_id();
        int receiverId = gchat->group_id();
        std::string message = gchat->message();
        int time = gchat->time();
#ifdef DEBUG
        std::cout << "this is a gchat ack!!!\n";
        std::cout << "senderId: " << senderId << "\n";
        std::cout << "groupId: " << receiverId << "\n";
        std::cout << "message: " << message << "\n";
        std::cout << "time: " << time << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKAFT1) {
        auto* aft1 = received_proto_message.mutable_aft1();
        int senderId = aft1->sender_id();
        int receiverId = aft1->receiver_id();
        bool status = aft1->status();
        std::string message = aft1->message();
#ifdef DEBUG
        std::cout << "this is a aft1 ack!!!\n";
        std::cout << "senderId: " << senderId << "\n";
        std::cout << "receiverId: " << receiverId << "\n";
        std::cout << "status: " << status << "\n";
        std::cout << "message: " << message << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKAFT2) {
        auto* aft2 = received_proto_message.mutable_aft2();
        int senderId = aft2->sender_id();
        int receiverId = aft2->receiver_id();
        std::string message = aft2->message();
#ifdef DEBUG
        std::cout << "this is a aft2 ack!!!\n";
        std::cout << "senderId: " << senderId << "\n";
        std::cout << "receiverId: " << receiverId << "\n";
        std::cout << "message: " << message << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKAGT1) {
        auto* agt1 = received_proto_message.mutable_agt1();
        int senderId = agt1->sender_id();
        int receiverId = agt1->group_id();
        bool status = agt1->status();
        std::string message = agt1->message();
#ifdef DEBUG
        std::cout << "this is a agt1 ack!!!\n";
        std::cout << "senderId: " << senderId << "\n";
        std::cout << "groupId: " << receiverId << "\n";
        std::cout << "status: " << status << "\n";
        std::cout << "message: " << message << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKAGT2) {
        auto* agt2 = received_proto_message.mutable_agt2();
        int senderId = agt2->sender_id();
        int receiverId = agt2->group_id();
        std::string message = agt2->message();
#ifdef DEBUG
        std::cout << "this is a agt2 ack!!!\n";
        std::cout << "senderId: " << senderId << "\n";
        std::cout << "receiverId: " << receiverId << "\n";
        std::cout << "message: " << message << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKUINFO) {
        auto* uinfo = received_proto_message.mutable_uinfo();
        int id = uinfo->id();
        std::string username = uinfo->username();
        std::string introduction = uinfo->introduction();
#ifdef DEBUG
        std::cout << "this is a uinfo ack!!!\n";
        std::cout << "id: " << id << "\n";
        std::cout << "username: " << username << "\n";
        std::cout << "introduction: " << introduction << "\n";
#endif
    } else if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_ACKGINFO) {
        auto* ginfo = received_proto_message.mutable_ginfo();
        int id = ginfo->id();
        std::string username = ginfo->groupname();
        std::string introduction = ginfo->introduction();
#ifdef DEBUG
        std::cout << "this is a ginfo ack!!!\n";
        std::cout << "id: " << id << "\n";
        std::cout << "username: " << username << "\n";
        std::cout << "introduction: " << introduction << "\n";
#endif
    }
    else {
        std::cout << "error\n";
    }
}
AsioTcpPtr MsgManager::getTcp() {
    return asioTcpPtr;
}
void MsgManager::setTcp(AsioTcpPtr tcpPtr) {
    asioTcpPtr = tcpPtr;
}
void MsgManager::send(im::protocol::Message &msg) {
    char buff[BUFFSIZE];
    msg.SerializeToArray(buff,BUFFSIZE);
    asioTcpPtr->send(buff, sizeof(buff));
}
void MsgManager::setTime(im::protocol::Message &msg) {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    msg.mutable_timestamp()->set_seconds(secs);
}

void MsgManager::sendURegister(im::protocol::Message &msg, const std::string &username, const std::string &password,
                               const std::string &introduction) {
    setTime(msg);
    msg.set_type(im::protocol::Message::UREGISTER);
    auto* uregister = msg.mutable_uregister();
    uregister->set_username(username);
    uregister->set_password(password);
    uregister->set_introduction(introduction);
}
void MsgManager::sendGRegister(im::protocol::Message &msg, int lordId, const std::string &groupname,
                               const std::string &introduction) {
    setTime(msg);
    msg.set_type(im::protocol::Message::GREGISTER);
    auto* gregister = msg.mutable_gregister();
    gregister->set_lord_id(lordId);
    gregister->set_groupname(groupname);
    gregister->set_introduction(introduction);
}
void MsgManager::sendLogin(im::protocol::Message &msg, int id, std::string password) {
    setTime(msg);
    msg.set_type(im::protocol::Message::LOGIN);
    auto* login = msg.mutable_login();
    login->set_user_id(id);
    login->set_password(password);
}
void MsgManager::sendUChat(im::protocol::Message &msg, int senderId, int receiverId, const std::string &message,
                           int time) {
    setTime(msg);
    msg.set_type(im::protocol::Message::UCHAT);
    auto* uchat = msg.mutable_uchat();
    uchat->set_sender_id(senderId);
    uchat->set_receiver_id(receiverId);
    uchat->set_message(message);
    uchat->set_time(time);
}
void MsgManager::sendGChat(im::protocol::Message &msg, int senderId, int groupId, const std::string &message,
                           int time) {
    setTime(msg);
    msg.set_type(im::protocol::Message::GCHAT);
    auto* gchat = msg.mutable_gchat();
    gchat->set_sender_id(senderId);
    gchat->set_group_id(groupId);
    gchat->set_message(message);
    gchat->set_time(time);
}
void MsgManager::sendAFriendF1(im::protocol::Message &msg, int senderId, int receiverId, const std::string &message,
                               int time) {
    setTime(msg);
    msg.set_type(im::protocol::Message::AFF1);
    auto* aff1 = msg.mutable_aff1();
    aff1->set_sender_id(senderId);
    aff1->set_receiver_id(receiverId);
    aff1->set_message(message);
    aff1->set_time(time);
}
void MsgManager::sendAFriendF2(im::protocol::Message &msg, int senderId, int receiverId, bool status,
                               const std::string &message, int time) {
    setTime(msg);
    msg.set_type(im::protocol::Message::AFF2);
    auto* aff2 = msg.mutable_aff2();
    aff2->set_sender_id(senderId);
    aff2->set_receiver_id(receiverId);
    aff2->set_status(status);
    aff2->set_message(message);
    aff2->set_time(time);
}
void MsgManager::sendAGroupF1(im::protocol::Message &msg, int senderId, int groupId, const std::string &message,
                              int time) {
    setTime(msg);
    msg.set_type(im::protocol::Message::AGF1);
    auto* agf1 = msg.mutable_agf1();
    agf1->set_sender_id(senderId);
    agf1->set_group_id(groupId);
    agf1->set_message(message);
    agf1->set_time(time);
}
void MsgManager::sendAGroupF2(im::protocol::Message &msg, int senderId, int groupId, bool status,
                              const std::string &message, int time) {
    setTime(msg);
    msg.set_type(im::protocol::Message::AGF2);
    auto* agf2 = msg.mutable_agf2();
    agf2->set_sender_id(senderId);
    agf2->set_group_id(groupId);
    agf2->set_status(status);
    agf2->set_message(message);
    agf2->set_time(time);
}
void MsgManager::sendSUser(im::protocol::Message &msg, int sendId, const std::string &message) {
    ;
}
void MsgManager::sendSGroup(im::protocol::Message &msg, int senderId, const std::string &message) {
    ;
}
void MsgManager::sendHeartBeat(im::protocol::Message &msg) {
    setTime(msg);
    msg.set_type(im::protocol::Message::HEARTBEAT);
    auto* heartbeat = msg.mutable_heartbeat();
    heartbeat->set_id(userId);
#ifdef DEBUG
    std::cout << "setting heartbeat message !!!\n";
#endif
}