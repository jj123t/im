//
// Created by 25421 on 2024/4/24.
//

#include "msgmanager.h"
#include "channel.h"
//#include <google/protobuf/timestamp.pb.h>
//#include <google/protobuf/util/time_util.h>
#include <sstream>
#include <iomanip>

MsgManager::MsgManager(EventLoop* _loop, NetworkBase* _sock) : loop(_loop), networkBase(_sock), channel(nullptr), readBuffer(nullptr) {
    channel = new Channel(loop, networkBase->getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> cb = std::bind(&MsgManager::paser, this, _sock);
    channel->setReadCallback(cb);
    readBuffer = std::shared_ptr<Buffer>();
}

void MsgManager::handleLoginRequest(im::protocol::Message &msg) {
    std::cout << "this is a login message !!!\n";
    auto* add_friend = msg.mutable_login_request();
    int userId = add_friend->user_id();
    std::string password = add_friend->password();
    std::cout << "userId is: " << userId << " password is: " << password << "\n";
}

void MsgManager::handleAddFriend(im::protocol::Message &msg) {
    std::cout << "this is a addfriend message !!!\n";
    auto* add_friend = msg.mutable_add_friend();
    int sendId = add_friend->sender_id();
    int recvId = add_friend->receiver_id();
    std::string text = add_friend->message();
    std::cout << "sendid is: " << sendId << " recvid is: " << recvId << " text is: " << text << "\n";
}

void MsgManager::handleAddGroup(im::protocol::Message &msg) {
    std::cout << "this is a addgroup message !!!\n";
    auto* add_group = msg.mutable_add_group();
    int sendId = add_group->sender_id();
    int groupId = add_group->group_id();
    std::string text = add_group->message();
    std::cout << "sendid is: " << sendId << " groupid is: " << groupId << " text is: " << text << "\n";
}

void MsgManager::handleSingleChat(im::protocol::Message &msg) {
    std::cout << "this is a single message !!!\n";
    auto* single_chat = msg.mutable_single_chat();
    int sendId = single_chat->sender_id();
    int recvId = single_chat->receiver_id();
    std::string text = single_chat->content();
    std::cout << "sendid is: " << sendId << " recvid is: " << recvId << " text is: " << text << "\n";
}

void MsgManager::handleGroupChat(im::protocol::Message &msg) {
    std::cout << "this is a group message !!!\n";
    auto* single_chat = msg.mutable_group_chat();
    int sendId = single_chat->sender_id();
    int groupId = single_chat->group_id();
    std::string text = single_chat->content();
    std::cout << "sendid is: " << sendId << " groupId is: " << groupId << " text is: " << text << "\n";
}

void MsgManager::handleSearchUser(im::protocol::Message &msg) {
    std::cout << "this is a search user message !!!\n";
    auto* single_chat = msg.mutable_search_user();
    int sendId = single_chat->sender_id();
    std::string text = single_chat->message();
    std::cout << "sendid is: " << sendId << " text is: " << text << "\n";
}

void MsgManager::handleSearchGroup(im::protocol::Message &msg) {
    std::cout << "this is a search group message !!!\n";
    auto* single_chat = msg.mutable_search_group();
    int sendId = single_chat->sender_id();
    std::string text = single_chat->message();
    std::cout << "sendid is: " << sendId << " text is: " << text << "\n";
}
void MsgManager::handleFileTransfer(im::protocol::Message &msg) {
    std::cout << "this is a filetransfer message !!!\n";
    auto* single_chat = msg.mutable_file_transfer();
    int sendId = single_chat->sender_id();
    int recvId = single_chat->receiver_id();
    std::string text = single_chat->file_hash();
    std::cout << "sendid is: " << sendId << " recvid is: " << recvId << " hash is: " << text << "\n";
}
void MsgManager::handleHeartBeat(im::protocol::Message &msg) {
    std::cout << "this is a heartbeat message !!!\n";
}

void MsgManager::paser(NetworkBase* nb) {

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    int BUFSIZE = 1024;
    char buff[BUFSIZE];
    nb->recv(buff, sizeof(buff));
    im::protocol::Message received_proto_message;
    received_proto_message.ParseFromArray(buff, sizeof(buff));
    std::time_t tt = received_proto_message.timestamp().seconds();
    std::tm * local_time = std::localtime(&tt);
    // 使用std::put_time格式化日期和时间
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    // 输出格式化后的时间字符串
    std::cout << "Timestamp as string: " << oss.str() << std::endl;
    received_proto_message.timestamp().seconds();

    if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_LOGIN_REQUEST) {
        handleLoginRequest(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_SINGLE_CHAT) {
        handleSingleChat(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_GROUP_CHAT) {
        handleGroupChat(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_ADD_FRIEND) {
        handleAddFriend(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_ADD_GROUP) {
        handleAddGroup(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_SEARCH_USER) {
        handleSearchUser(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_SEARCH_GROUP) {
        handleSearchGroup(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_FILE_TRANSFER) {
        handleFileTransfer(received_proto_message);
    }
    else if (received_proto_message.type() == im::protocol::Message_Type::Message_Type_HEARTBEAT) {
        handleHeartBeat(received_proto_message);
    }
    else {
        std::cout << "error parser !!!\n";
        exit(0);
    }

//    std::cout << received_proto_message.type() << "\n";
//    auto* single_chat = received_proto_message.mutable_single_chat();
//    std::cout << single_chat->content() << "\n";

    deleteConnectionCallback(nb);
}

MsgManager::~MsgManager() {
}

void MsgManager::setDeleteConnectionCallback(std::function<void(NetworkBase*)> _cb){
    deleteConnectionCallback = _cb;
}