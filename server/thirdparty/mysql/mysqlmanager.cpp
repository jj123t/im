//
// Created by 25421 on 2024/4/24.
//

#include "thirdparty/mysql/mysqlmanager.h"

sql::mysql::MySQL_Driver* MysqlManager::driver_ = nullptr;
sql::Connection* MysqlManager::con_ = nullptr;
std::string MysqlManager::host_;
std::string MysqlManager::user_;
std::string MysqlManager::password_;
std::string MysqlManager::database_;

void MysqlManager::init(const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
    host_ = host;
    user_ = user;
    password_ = password;
    database_ = database;
    if(connect()) {
        std::cout << "success connect mysql!!!\n";
    }
    else {
        std::cout << "failed connect mysql!!!\n";
    }
}

bool MysqlManager::connect() {
    try {
        driver_ =  sql::mysql::get_driver_instance();
        con_ = driver_->connect(host_, user_, password_);
        con_->setSchema(database_);
        return true;
    } catch (sql::SQLException& e) {
        return false;
    }
}

int MysqlManager::userRegister(const std::string& username, const std::string& password, const std::string& introduction) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into user_info (username, password, introduction, last_login) values (?, ?, ?, ?)");
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        pstmt->setString(3, introduction);
        pstmt->setInt(4, std::time(0));
        pstmt->executeUpdate();
        pstmt = con_->prepareStatement("SELECT LAST_INSERT_ID()");
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            id = res->getInt(1);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return id;
}

int MysqlManager::userLogin(int userId, const std::string& password) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt =
//                con_->prepareStatement("update user_info set last_login = 10 where id = ?");
//        unix_timestamp(now())
//        pstmt->setInt(1, userId);
//        pstmt->executeUpdate();
        pstmt = con_->prepareStatement("select id from user_info where id = ? and password = ? ");
        pstmt->setInt(1, userId);
        pstmt->setString(2, password);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            id = res->getInt("id");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return id != -1;
}

int MysqlManager::setUserHB(int userId, int time) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("update user_info set last_login = ? where id = ?");
        pstmt->setInt(1, time);
        pstmt->setInt(2, userId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::getUserHB(int userId) {
    int t = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select last_login from user_info where id = ?");
        pstmt->setInt(1, userId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            t = res->getInt("last_login");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return t;
}

int MysqlManager::delUser(int userId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from user_info where id = ?");
        pstmt->setInt(1, userId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::updUser(int userId, const std::string &username, const std::string &password,
                          const std::string &introduction) {
    try {
        sql::PreparedStatement* pstmt =
                con_->prepareStatement("update user_info set username = ?, password = ?, introduction = ? where id = ?");
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        pstmt->setString(3, introduction);
        pstmt->setInt(4, userId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::groupRegister(int lordId, const std::string& groupname, const std::string& introduction) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into group_info (groupname, lord_id, introduction) values (?, ?, ?)");
        pstmt->setString(1, groupname);
        pstmt->setInt(2, lordId);
        pstmt->setString(3, introduction);
        pstmt->executeUpdate();
        pstmt = con_->prepareStatement("SELECT LAST_INSERT_ID()");
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            id = res->getInt(1);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return id;
}

int MysqlManager::delGroup(int groupId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from group_info where group_id = ?");
        pstmt->setInt(1, groupId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::updGroup(int lordId, int groupId, const std::string &groupname, const std::string &introduction) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("update group_info set lord_id = ?, groupname = ?, introduction = ? where id = ?");
        pstmt->setInt(1, lordId);
        pstmt->setString(2, groupname);
        pstmt->setString(3, introduction);
        pstmt->setInt(4, groupId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

MysqlManager::GroupInfo MysqlManager::getGroupInfo(int groupId) {
    MysqlManager::GroupInfo groupInfo;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select lord_id, groupname, introduction from group_info where id = ?");
        pstmt->setInt(1, groupId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            groupInfo.id = groupId;
            groupInfo.lordId = res->getInt("lord_id");
            groupInfo.groupname = res->getString("groupname");
            groupInfo.introduction = res->getString("introduction");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groupInfo;
}

MysqlManager::UserInfo MysqlManager::getUserInfo(int userId) {
    MysqlManager::UserInfo userInfo;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select username, introduction from user_info where id = ?");
        pstmt->setInt(1, userId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            userInfo.id = userId;
            userInfo.username = res->getString("username");
            userInfo.introduction = res->getString("introduction");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return userInfo;
}

int MysqlManager::addFriend(int id1, int id2) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into friend (id1, id2) VALUES (?, ?)");
        pstmt->setInt(1, id1);
        pstmt->setInt(2, id2);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::delFriend(int id1, int id2) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from friend where id1 = ? and id2 = ?");
        pstmt->setInt(1, id1);
        pstmt->setInt(2, id2);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::postFriend(int senderId, int receiverId, const std::string& message, int time) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into apply_friend (sender_id, receiver_id, message, time) VALUES (?, ?, ?, ?)");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->setString(3, message);
        pstmt->setInt(4, time);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

std::vector<MysqlManager::FriendRecord> MysqlManager::getFriend(int id, int time) {
    std::vector<FriendRecord> friendRecords;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select sender_id, receiver_id, message, time from apply_friend where receiver_id = ? and time >= ?");
        pstmt->setInt(1, id);
        pstmt->setInt(2, time);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            FriendRecord tmp;
            tmp.senderId = res->getInt("sender_id");
            tmp.receiverId = id;
            tmp.content = res->getString("message");
            tmp.time = res->getInt("time");
            friendRecords.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return friendRecords;
}

std::vector<MysqlManager::UserInfo> MysqlManager::getFriends(int id) {
    std::vector<UserInfo> friends;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select * from friend where id1 = ? or id2 = ? ");
        pstmt->setInt(1, id);
        pstmt->setInt(2, id);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            UserInfo tmp;
            int id1 = res->getInt("id1");
            int id2 = res->getInt("id2");
            if (id1 == id) tmp = getUserInfo(id2);
            else tmp = getUserInfo(id1);
            friends.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return friends;
}

int MysqlManager::addGroup(int id, int groupId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into group_relation (group_id, id) VALUES (?, ?)");
        pstmt->setInt(1, groupId);
        pstmt->setInt(2, id);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::delGroup(int senderId, int groupId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from group_relation where group_id = ? and id = ?");
        pstmt->setInt(1, groupId);
        pstmt->setInt(2, senderId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::getLord(int groupId) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select lord_id from group_info where id = ?");
        pstmt->setInt(1, groupId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            id = res->getInt("lord_id");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return id;
}

int MysqlManager::postGroup(int senderId, int groupId, const std::string &message, int time) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into apply_group (sender_id, group_id, message, time) values (?, ?, ?, ?)");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, groupId);
        pstmt->setString(3, message);
        pstmt->setInt(4, time);
        pstmt->executeQuery();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

std::vector<MysqlManager::GroupRecord> MysqlManager::getGroup(int groupId, int time) {
    std::vector<GroupRecord> groupRecord;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select sender_id, group_id, message, time from apply_group where group_id = ? and time >= ?");
        pstmt->setInt(1, groupId);
        pstmt->setInt(2, time);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            GroupRecord tmp;
            tmp.senderId = res->getInt("sender_id");
            tmp.groupId = groupId;
            tmp.content = res->getInt("message");
            tmp.time = res->getInt("time");
            groupRecord.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groupRecord;
}


std::vector<MysqlManager::GroupInfo> MysqlManager::getGroupsU(int id) {
    std::vector<GroupInfo> groups;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select * from group_relation where id = ? ");
        pstmt->setInt(1, id);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            GroupInfo tmp;
            int groupId = res->getInt("group_id");
            tmp = getGroupInfo(groupId);
            groups.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groups;
}

std::vector<MysqlManager::GroupInfo> MysqlManager::getGroupsL(int id) {
    std::vector<GroupInfo> groups;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select * from group_info where lord_id = ? ");
        pstmt->setInt(1, id);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            GroupInfo tmp;
            int groupId = res->getInt("id");
            tmp = getGroupInfo(groupId);
            groups.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groups;
}

std::vector<MysqlManager::UserInfo> MysqlManager::getGroupMember(int id) {
    std::vector<UserInfo> groupMember;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select id from group_info where lord_id = ? ");
        pstmt->setInt(1, id);
        sql::ResultSet* res = pstmt->executeQuery();
//        while (res->next()) {
//            UserInfo tmp;
//            int groupId = res->getInt("id");
//            tmp = getUserInfo(groupId);
//            groupMember.push_back(tmp);
//        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groupMember;
}

int MysqlManager::addUserChat(int senderId, int receiverId, const std::string &content, std::time_t time) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into user_chat (sender_id, receiver_id, message, time) values (?, ?, ?, ?)");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->setString(3, content);
        pstmt->setInt(4, time);
        pstmt->executeUpdate();
        pstmt = con_->prepareStatement("SELECT LAST_INSERT_ID()");
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            id = res->getInt(1);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return id;
}

int MysqlManager::delUserChat(int chatId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from user_chat where id = ?");
        pstmt->setInt(1, chatId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}

int MysqlManager::addGroupChat(int senderId, int groupId, const std::string &content, std::time_t time) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into group_chat (sender_id, group_id, message, time) values (?, ?, ?, ?)");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, groupId);
        pstmt->setString(3, content);
        pstmt->setInt(4, time);
        pstmt->executeUpdate();
        pstmt = con_->prepareStatement("SELECT LAST_INSERT_ID()");
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            id = res->getInt(1);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return id;
}

int MysqlManager::delGroupChat(int chatId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from group_chat where id = ?");
        pstmt->setInt(1, chatId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;

}

std::vector<MysqlManager::UserChatRecord> MysqlManager::getUserChat(int senderId, int receiverId, time_t startTimestamp, time_t endTimestamp) {
    std::vector<MysqlManager::UserChatRecord> userChatRecord;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select sender_id, receiver_id, message, time from user_chat where receiver_id = ? and  ? <= time and time <=  ?");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->setInt(3, startTimestamp);
        pstmt->setInt(4, endTimestamp);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            MysqlManager::UserChatRecord tmp;
            tmp.senderId = res->getInt("sender_id");
            tmp.receiverId = res->getInt("receiver_id");
            tmp.content = res->getString("message");
            tmp.time = res->getInt("time");
            userChatRecord.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return userChatRecord;
}

std::vector<MysqlManager::UserChatRecord> MysqlManager::getUserChatOne(int receiverId, time_t startTimestamp, time_t endTimestamp) {
    std::vector<MysqlManager::UserChatRecord> userChatRecord;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select sender_id, receiver_id, message, time from user_chat where receiver_id = ? and  ? <= time and time <=  ?");
        pstmt->setInt(1, receiverId);
        pstmt->setInt(2, startTimestamp);
        pstmt->setInt(3, endTimestamp);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            MysqlManager::UserChatRecord tmp;
            tmp.senderId = res->getInt("sender_id");
            tmp.receiverId = res->getInt("receiver_id");
            tmp.content = res->getString("message");
            tmp.time = res->getInt("time");
            userChatRecord.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return userChatRecord;
}

std::vector<MysqlManager::GroupChatRecord> MysqlManager::getGroupChat(int senderId, int groupId, time_t startTimestamp, time_t endTimestamp) {
    std::vector<MysqlManager::GroupChatRecord> groupChatRecord;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select group_id, sender_id, message, time from group_chat where group_id = ? and sender_id = ? and  ? <= time and time <= ? ");
        pstmt->setInt(1, groupId);
        pstmt->setInt(2, senderId);
        pstmt->setInt(3, startTimestamp);
        pstmt->setInt(4, endTimestamp);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            MysqlManager::GroupChatRecord tmp;
            tmp.groupId = res->getInt("group_id");
            tmp.senderId = res->getInt("sender_id");
            tmp.content = res->getString("message");
            tmp.time = res->getInt("time");
            groupChatRecord.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groupChatRecord;
}
