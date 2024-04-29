//
// Created by 25421 on 2024/4/24.
//

#include "mysqlmanager.h"

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

// 添加 删除好友
int MysqlManager::makeFriend(int senderId, int receiverId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into friend_ship (one_id, two_id) VALUES (?, ?)");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}
int MysqlManager::tieFriend(int senderId, int receiverId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from friend_ship where one_id = ? and two_id = ?");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}
// 加入 退出群组
int MysqlManager::makeGroup(int senderId, int groupId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into group_ship (group_id, menber_id) VALUES (?, ?)");
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
int MysqlManager::tieGroup(int senderId, int groupId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from group_ship where group_id = ? and menber_id = ?");
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
// 用户   注册，登陆，删除，更新
int MysqlManager::userRegister(const std::string& username, const std::string& password, const std::string& introduction) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into user_info (username, password, introduction) values (?, ?, ?)");
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        pstmt->setString(3, introduction);
        pstmt->executeUpdate();
        delete pstmt;
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
        sql::PreparedStatement* pstmt = con_->prepareStatement("select id from user_info where id = ? and password = ? ");
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
    return id;
}

int MysqlManager::userDelete(int userId) {
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
int MysqlManager::userUpdate(int userId, const std::string& username, const std::string& password, const std::string& introduction) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("update user_info set username = ?, password = ?, introduction = ? where id = ?");
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
MysqlManager::UserInfo MysqlManager::getUserInfo(int userId) {
    MysqlManager::UserInfo userInfo;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select username, introduction from user_info where id = ?");
        pstmt->setInt(1, userId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            userInfo.username = res->getString("username");
            userInfo.introduction = res->getString("introduction");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return userInfo;
}
// 群组   注册，登陆，删除，更新
int MysqlManager::groupRegister(int lord_id, const std::string& groupname, const std::string& introduction) {
    int id = -1;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into group_info (groupname, lord_id, introduction) values (?, ?, ?)");
        pstmt->setString(1, groupname);
        pstmt->setInt(2, lord_id);
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
int MysqlManager::groupDelete(int groupId) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("delete from group_info where id = ?");
        pstmt->setInt(1, groupId);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}
int MysqlManager::groupUpdate(int groupId, const std::string& groupname, const std::string& introduction) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("update user_info set groupname = ?, introduction = ? where id = ?");
        pstmt->setString(1, groupname);
        pstmt->setString(2, introduction);
        pstmt->setInt(3, groupId);
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
        sql::PreparedStatement* pstmt = con_->prepareStatement("select groupname, introduction from group_info where id = ?");
        pstmt->setInt(1, groupId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            groupInfo.groupname = res->getString("groupname");
            groupInfo.introduction = res->getString("introduction");
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groupInfo;
}
// 用户聊天记录
int MysqlManager::insertUserChat(int senderId, int receiverId, const std::string& content, std::time_t time) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into friend_chat (sender_id, receiver_id, content, time) values (?, ?, ?, FROM_UNIXTIME(?) )");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->setString(3, content);
        pstmt->setInt(4, time);
        pstmt->executeQuery();
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
        sql::PreparedStatement* pstmt = con_->prepareStatement("select sender_id, receiver_id, content, time from friend_chat sender_id = ? and receiver_id = ? and  from_unixtime(?) <= time and time <=  from_unixtime(?)");
        pstmt->setInt(1, senderId);
        pstmt->setInt(2, receiverId);
        pstmt->setInt(3, startTimestamp);
        pstmt->setInt(4, endTimestamp);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            MysqlManager::UserChatRecord tmp;
            tmp.senderId = res->getInt("sender_id");
            tmp.receiverId = res->getInt("receiver_id");
            tmp.content = res->getString("content");
            tmp.time = res->getInt("time");
            userChatRecord.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return userChatRecord;
}
// 群组聊天记录
int MysqlManager::insertGroupChat(int senderId, int groupId, const std::string& content, std::time_t time) {
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("insert into group_chat (group_id, sender_id, content, time) values (?, ?, ?, FROM_UNIXTIME(?) )");
        pstmt->setInt(1, groupId);
        pstmt->setInt(2, senderId);
        pstmt->setString(3, content);
        pstmt->setInt(4, time);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    return true;
}
std::vector<MysqlManager::GroupChatRecord> MysqlManager::getGroupChat(int senderId, int groupId, time_t startTimestamp, time_t endTimestamp) {
    std::vector<MysqlManager::GroupChatRecord> groupChatRecord;
    try {
        sql::PreparedStatement* pstmt = con_->prepareStatement("select group_id, sender_id, content, time from friend_chat group_id = ? and sender_id = ? and  from_unixtime(?) <= time and time <=  from_unixtime(?)");
        pstmt->setInt(1, groupId);
        pstmt->setInt(2, senderId);
        pstmt->setInt(3, startTimestamp);
        pstmt->setInt(4, endTimestamp);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            MysqlManager::GroupChatRecord tmp;
            tmp.groupId = res->getInt("group_id");
            tmp.senderId = res->getInt("sender_id");
            tmp.content = res->getString("content");
            tmp.time = res->getInt("time");
            groupChatRecord.push_back(tmp);
        }
        delete pstmt;
    } catch (sql::SQLException& e) {
        std::cout << "MySQL Error: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
    }
    return groupChatRecord;
}
