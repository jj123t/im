//
// Created by 25421 on 2024/5/1.
//

#ifndef TESTMYSQL_NETWORKBASE_H
#define TESTMYSQL_NETWORKBASE_H
#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <array>
#include <atomic>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <cstddef>

class AsioTcp;
using AsioTcpPtr = boost::shared_ptr<AsioTcp>;
class AsioTcpServer;
using AsioTcpServerPtr = boost::shared_ptr<AsioTcpServer>;
class AsioTcp : public boost::enable_shared_from_this<AsioTcp>
{
public:
    constexpr static size_t RECV_SIZE = 2048; ///< TCP每次最多读取数据量
    using Tcp = boost::asio::ip::tcp;
    using Bufffer = std::array<char, RECV_SIZE>;
    using IpAddress = boost::asio::ip::address;
    using ReadCallback = std::function<bool(
            const boost::system::error_code&, const char*, const size_t, AsioTcpPtr )>;
    using WriteCallback = std::function<bool(
            const boost::system::error_code&, const size_t, const size_t, AsioTcpPtr )>;
    static AsioTcpPtr create( boost::asio::io_context& io_context,
                              ReadCallback             readcallback = nullptr,
                              const int                id = -1,
                              WriteCallback            writecallback = nullptr );
    ~AsioTcp();
    static AsioTcpPtr create( boost::asio::io_context& io_context,
                              const std::string&       ip,
                              const int                port,
                              ReadCallback             readcallback = nullptr,
                              const int                id = -1,
                              WriteCallback            writecallback = nullptr );
    void startReceive();
    void disconnect();
    void send( const void* data, size_t len );
    void setId( int id );
    const int getId() const;
    void setReadCallback( ReadCallback callback );
    void setWriteCallback( WriteCallback callback );
    Tcp::socket& getSocket();
    IpAddress getLocalAddress() const;
    IpAddress getRemoteAddress() const;
    AsioTcp& operator=( AsioTcp&& ) = delete;
    AsioTcp& operator=( const AsioTcp& ) = delete;
private:
    AsioTcp( boost::asio::io_context& io_context,
             ReadCallback&            readcallback,
             WriteCallback&           writecallback,
             int                      id );
    void handle_write( const boost::system::error_code& ec, size_t sendlen, size_t origlen );
    int           m_id;            ///< 连接ID
    Tcp::socket   m_socket;        ///< ASIOTCP套接字
    ReadCallback  m_readcallback;  ///< 异步读取的函数指针
    Bufffer       m_recvBuf;       ///< 接收缓存
    WriteCallback m_writecallback; ///< 异步发送的函数指针
};

class AsioTcpServer : public boost::enable_shared_from_this<AsioTcpServer>
{
public:
    constexpr static int LISTEN_BACKLOG = 32;
    using IoServer = boost::asio::io_context;
    using AcceptCallback = std::function<void( AsioTcpPtr& )>;
    using ReadCallBack = AsioTcp::ReadCallback;
    using Tcp = boost::asio::ip::tcp;
    using AcceptorPtr = boost::shared_ptr<Tcp::acceptor>;
    using IpAddress = boost::asio::ip::address;
    static AsioTcpServerPtr create( IoServer&          ioContext,
                                    const std::string& ip,
                                    const int          port,
                                    ReadCallBack       onRead = nullptr,
                                    AcceptCallback     onAccept = nullptr,
                                    const int          id = -1,
                                    const bool         reuseAddr = false );

    AsioTcpServer( AsioTcpServer&& ) = delete;
    AsioTcpServer& operator=( AsioTcpServer&& ) = delete;
    AsioTcpServer( const AsioTcpServer& ) = delete;
    AsioTcpServer& operator=( const AsioTcpServer& ) = delete;
    void setId( int id );
    const int getId() const;
    IpAddress getLocalAddress() const;
    void setOnAcceptCallback( AcceptCallback onAccept );
    void setOnReadCallback( ReadCallBack onRead );
private:
    AsioTcpServer( const int         id,
                   IoServer&         io,
                   ReadCallBack      onRead,
                   AcceptorPtr       acceptor,
                   AcceptCallback    onAccept,
                   const std::string ip,
                   const int         port );
    void handleAccept( AsioTcpPtr new_connection, const boost::system::error_code& error );
    void startAccept();
    int               m_id;               ///< 连接ID
    IoServer&         m_io;               ///< ASIO的调度器
    ReadCallBack      m_onReadCallback;   ///< ASIO的异步读取回调函数
    AcceptorPtr       m_acceptor;         ///< ASIO的连接接收器
    AcceptCallback    m_onAcceptCallback; ///< accept的异步回调函数
    const std::string m_ip;               ///< 监听地址
    const int         m_port;             ///< 监听端口
};


class AsioTcpManager
{
public:
    using IoServer = boost::asio::io_context;
    using Tcp = boost::asio::ip::tcp;
    using ReadCallback = AsioTcp::ReadCallback;
    using AcceptCallback = AsioTcpServer::AcceptCallback;
    using TcpAccepter = std::shared_ptr<Tcp::acceptor>;
    AsioTcpManager( IoServer& ioContext );
    ~AsioTcpManager();
    int addListen( const std::string ip,
                   const int         port,
                   AcceptCallback    onAccept = nullptr,
                   ReadCallback      onRead = nullptr );
    int addTransmission( const std::string& ip, const int port, ReadCallback callback );
    int addTransmission( AsioTcpPtr& connection );
    bool removeConnection( AsioTcpPtr& connection );
    bool removeConnection( int id );
    AsioTcpServerPtr getServer( int id );
    AsioTcpPtr getClient( int id );
    AsioTcpManager( const AsioTcpManager& ) = delete;
    AsioTcpManager& operator=( const AsioTcpManager& ) = delete;
    AsioTcpManager( AsioTcpManager&& ) = delete;
    AsioTcpManager& operator=( AsioTcpManager&& ) = delete;
private:
    int getNextConnectionId();
    void acceptNewConnection( AcceptCallback acceptCallback, AsioTcpPtr connection );
    IoServer&                       m_io;          ///< ASIO调度器
    std::mutex                      m_lock;        ///< 容器互斥锁
    std::map<int, AsioTcpPtr>       m_connections; ///< TCP连接的存储容器
    std::map<int, AsioTcpServerPtr> m_listeners;   ///< 监听连接的存储容器
    static std::atomic_int          m_netxId;      ///< 新建连接的ID
};

#endif //TESTMYSQL_NETWORKBASE_H
