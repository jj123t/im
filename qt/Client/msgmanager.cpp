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
void MsgManager::paser(const char* buf, int len) {
    std::cout << buf << "\n";
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    im::protocol::Ack received_proto_message;
    received_proto_message.ParseFromArray(buf, 1024);
    std::time_t tt = received_proto_message.timestamp().seconds();
    std::tm * local_time = std::localtime(&tt);
    // 使用std::put_time格式化日期和时间
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    // 输出格式化后的时间字符串
    std::cout << "Timestamp as string: " << oss.str() << std::endl;
    received_proto_message.timestamp().seconds();
    if (received_proto_message.type() == im::protocol::Ack_Type::Ack_Type_LOGIN_REQUEST) {
        std::cout << "this is a login ack!!!\n";
    }
    else {
        std::cout << "no type!!!\n";
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

void MsgManager::sendLoginRequest(im::protocol::Message &msg, int id, std::string password) {
    setTime(msg);
    msg.set_type(im::protocol::Message::LOGIN_REQUEST);
    std::cout << "setting login message !!!\n";
    auto* loginRequest = msg.mutable_login_request();
    loginRequest->set_user_id(id);
    loginRequest->set_password(password);
}

void MsgManager::sendAddFriend(im::protocol::Message &msg, int senderId, int receiverId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::ADD_FRIEND);
    std::cout << "setting addfriend message !!!\n";
    auto* add_friend = msg.mutable_add_friend();
    add_friend->set_sender_id(senderId);
    add_friend->set_receiver_id(receiverId);
    add_friend->set_message(message);
}

void MsgManager::sendAddGroup(im::protocol::Message &msg, int senderId, int groupId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::ADD_GROUP);
    std::cout << "setting addgroup message !!!\n";
    auto* add_group = msg.mutable_add_group();
    add_group->set_sender_id(senderId);
    add_group->set_group_id(groupId);
    add_group->set_message(message);
}

void MsgManager::sendSingleChat(im::protocol::Message &msg, int senderId, int receiverId, const std::string& content) {
    setTime(msg);
    msg.set_type(im::protocol::Message::SINGLE_CHAT);
    std::cout << "setting single message !!!\n";
    auto* single_chat = msg.mutable_single_chat();
    single_chat->set_sender_id(senderId);
    single_chat->set_receiver_id(receiverId);
    single_chat->set_content(content);
}

void MsgManager::sendGroupChat(im::protocol::Message &msg, int senderId, int groupId, const std::string& content) {
    setTime(msg);
    msg.set_type(im::protocol::Message::GROUP_CHAT);
    std::cout << "setting group message !!!\n";
    auto* group_chat = msg.mutable_group_chat();
    group_chat->set_sender_id(senderId);
    group_chat->set_group_id(groupId);
    group_chat->set_content(content);
}

void MsgManager::sendSearchUser(im::protocol::Message &msg, int sendId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::SEARCH_USER);
    std::cout << "setting search user message !!!\n";
    auto* searchUser = msg.mutable_search_user();
    searchUser->set_sender_id(sendId);
    searchUser->set_message(message);
}

void MsgManager::sendSearchGroup(im::protocol::Message &msg, int senderId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::SEARCH_GROUP);
    std::cout << "setting search group message !!!\n";
    auto* searchGroup = msg.mutable_search_group();
    searchGroup->set_sender_id(senderId);
    searchGroup->set_message(message);
}

void MsgManager::sendFileTransfer(im::protocol::Message &msg, int senderId, int receiverId) {
    setTime(msg);
    msg.set_type(im::protocol::Message::FILE_TRANSFER);
    std::cout << "setting filetransfer message !!!\n";
    auto* fileTransfer = msg.mutable_file_transfer();
    fileTransfer->set_sender_id(senderId);
    fileTransfer->set_receiver_id(receiverId);
}
void MsgManager::sendHeartBeat(im::protocol::Message &msg) {
    setTime(msg);
    msg.set_type(im::protocol::Message::HEARTBEAT);
    std::cout << "setting heartbeat message !!!\n";
}