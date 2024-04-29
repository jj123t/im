//
// Created by 25421 on 2024/4/26.
//

#include <common/network/networkbase.h>
#include <thirdparty/protobuf/msg.pb.h>

void setTime(im::protocol::Message &msg) {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    msg.mutable_timestamp()->set_seconds(secs);
}

void sendLoginRequest(im::protocol::Message &msg, int id, std::string password) {
    setTime(msg);
    msg.set_type(im::protocol::Message::LOGIN_REQUEST);
    std::cout << "setting login message !!!\n";
    auto* loginRequest = msg.mutable_login_request();
    loginRequest->set_user_id(id);
    loginRequest->set_password(password);
}

void sendAddFriend(im::protocol::Message &msg, int senderId, int receiverId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::ADD_FRIEND);
    std::cout << "setting addfriend message !!!\n";
    auto* add_friend = msg.mutable_add_friend();
    add_friend->set_sender_id(senderId);
    add_friend->set_receiver_id(receiverId);
    add_friend->set_message(message);
}

void sendAddGroup(im::protocol::Message &msg, int senderId, int groupId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::ADD_GROUP);
    std::cout << "setting addgroup message !!!\n";
    auto* add_group = msg.mutable_add_group();
    add_group->set_sender_id(senderId);
    add_group->set_group_id(groupId);
    add_group->set_message(message);
}

void sendSingleChat(im::protocol::Message &msg, int senderId, int receiverId, const std::string& content) {
    setTime(msg);
    msg.set_type(im::protocol::Message::SINGLE_CHAT);
    std::cout << "setting single message !!!\n";
    auto* single_chat = msg.mutable_single_chat();
    single_chat->set_sender_id(senderId);
    single_chat->set_receiver_id(receiverId);
    single_chat->set_content(content);
}

void sendGroupChat(im::protocol::Message &msg, int senderId, int groupId, const std::string& content) {
    setTime(msg);
    msg.set_type(im::protocol::Message::GROUP_CHAT);
    std::cout << "setting group message !!!\n";
    auto* group_chat = msg.mutable_group_chat();
    group_chat->set_sender_id(senderId);
    group_chat->set_group_id(groupId);
    group_chat->set_content(content);
}

void sendSearchUser(im::protocol::Message &msg, int sendId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::SEARCH_USER);
    std::cout << "setting search user message !!!\n";
    auto* searchUser = msg.mutable_search_user();
    searchUser->set_sender_id(sendId);
    searchUser->set_message(message);
}

void sendSearchGroup(im::protocol::Message &msg, int senderId, const std::string& message) {
    setTime(msg);
    msg.set_type(im::protocol::Message::SEARCH_GROUP);
    std::cout << "setting search group message !!!\n";
    auto* searchGroup = msg.mutable_search_group();
    searchGroup->set_sender_id(senderId);
    searchGroup->set_message(message);
}

void sendFileTransfer(im::protocol::Message &msg, int senderId, int receiverId) {
    setTime(msg);
    msg.set_type(im::protocol::Message::FILE_TRANSFER);
    std::cout << "setting filetransfer message !!!\n";
    auto* fileTransfer = msg.mutable_file_transfer();
    fileTransfer->set_sender_id(senderId);
    fileTransfer->set_receiver_id(receiverId);
}
void sendHeartBeat(im::protocol::Message &msg) {
    setTime(msg);
    msg.set_type(im::protocol::Message::HEARTBEAT);
    std::cout << "setting heartbeat message !!!\n";
}

int main() {
    std::vector<im::protocol::Message> vt(10);
    sendLoginRequest(vt[0], 1, "password");
    sendAddFriend(vt[1], 1, 2, "add frient");
    sendAddGroup(vt[2], 1, 2, "add group");
    sendSingleChat(vt[3], 1, 2, "singchat");
    sendGroupChat(vt[4], 1, 2, "groupchat");
    sendSearchUser(vt[5], 1, "search user");
    sendSearchGroup(vt[6], 1, "search group");
    sendFileTransfer(vt[7], 1, 2);
    sendHeartBeat(vt[8]);
    for (int i = 0; i <= 8; i++) {
        im::protocol::Message& message = vt[i];
        int BUFFSIZE = 1024;
        char buff[BUFFSIZE];
        message.SerializeToArray(buff,BUFFSIZE);
        NetworkBase t("127.0.0.1", 9000, NetworkType::TCPTRANSMISSION);
        t.send(buff, sizeof(buff));
//        sleep(5);
    }

    return 0;
}