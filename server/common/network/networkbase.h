//
// Created by 25421 on 2024/4/24.
//

#ifndef IM_NETWORKBASE_H
#define IM_NETWORKBASE_H

#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <fcntl.h>
#include <csignal>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <cstring>
#include <netinet/tcp.h>
#include <functional>

enum NetworkType : int
{
    TCPTRANSMISSION = 0, ///< 普通的TCP连接，包括accept产生的连接和客户端连接
    TCPSERVER,           ///< TCP监听连接，用于接收accept
    UDPTRANSMISSION,     ///< 普通的UDP连接
    UDPSERVER,           ///< UDP监听连接
    MULTICASTRECEIVER,   ///< 组播接收连接
    MULTICASTSENDER,     ///< 组播发送连接
    CLOSED,              ///< 连接已关闭
    NETWORKTYPECOUNT,    ///< 网络类型数量
};

class NetworkBase;
using NetworkBasePtr = std::shared_ptr<NetworkBase>;

class NetworkBase
{
public:
    NetworkBase( const std::string& ip,
                 const int          port,
                 const NetworkType  type,
                 const bool         nonblock = false,
                 const std::string  multicastInterface = "",
                 const std::string  sourceIP = "",
                 int                id = 0 );
    NetworkBase( int fd, NetworkType type, std::string ip = "", int port = -1, int id = -1 );
    NetworkBase( NetworkBase&& other );
    ~NetworkBase();
    NetworkBase& operator=( NetworkBase&& other );
    int getFd() const;
    const std::string& getIp() const;
    const int getPort() const;
    const NetworkType getType() const;
    const std::string& getMulticastSourceIP() const;
    const std::string& getMulticastInterface() const;
    const struct sockaddr_in& getSocketAddr() const;
    bool setup();
    bool release( bool initiator = true );
    bool listen();
    int accept();
    int accept( struct sockaddr_in& clnt_addr );
    NetworkBasePtr accept( int sessionid );
    ssize_t send( const char* data, size_t size );
    ssize_t sendTo( struct sockaddr_in& sendAddr, const void* data, size_t size );
    ssize_t recv( char* buf, size_t bufsize );
    ssize_t recvFrom( struct sockaddr_in& recvAddr, char* buf, size_t bufsize );
    virtual void handle(){};
    NetworkBase( NetworkBase& ) = delete;
    NetworkBase& operator=( NetworkBase& ) = delete;
    bool setNonBlock( bool nonblock );
    bool setNoDelay( bool nodelay );
    void setBacklog( int backlog );
    bool setReUseAddr();
    bool setLinger();
    void setSessionID( int id );
    int getID();
    void setDelConnCallBack(std::function<void(NetworkBase*)>);
private:
    bool create();
    bool bind();
    bool setSockaddrIn();
    bool connect();
    bool joinMulticastGroup();
    int         m_fd;       ///< 套接字的文件描述符
    std::string m_ip;       ///< 监听的IP或者要连接的对端IP
    int         m_port;     ///< 监听的端口或者要连接的对端端口
    NetworkType m_type;     ///< 网络类型
    bool        m_nonBlock; ///< 是否是非阻塞的
    int         m_backlog;  ///< backlog参数
    bool        m_accepted; ///< 是否是accept产生的连接
    int         m_id;       ///< 连接ID
    std::string m_sourceIP;  ///< 组播的源地址
    std::string m_interface; ///< 组播的网卡地址
    struct sockaddr_in m_socketAddr; ///< socket地址
    std::function<void(NetworkBase*)> delConnCallback;
};

#endif //IM_NETWORKBASE_H
