//
// Created by 25421 on 2024/5/1.
//

#include "networkbase.h"
#include <thread>
#include <iostream>
//#include <libcaf.h>

#define LOG_ERROR std::cout
#define LOG_INFO std::cout
#define LOG_ENDL std::endl

using error_code = boost::system::error_code;

namespace {
    auto asioReadDefault =
            []( const error_code& ec, const char* data, const size_t size, AsioTcpPtr tcp ) {
                return true;
            };
    auto asioWriteDefault =
            []( const error_code& ec, const size_t sentSize, const size_t size2Send, AsioTcpPtr tcp ) {
                return true;
            };
} // namespace

AsioTcpPtr AsioTcp::create( boost::asio::io_context& io_context,
                            ReadCallback             readcallback,
                            const int                id,
                            WriteCallback            writecallback )
{
    return AsioTcpPtr( new AsioTcp( io_context, readcallback, writecallback, id ) );
}

AsioTcpPtr AsioTcp::create( boost::asio::io_context& io_context,
                            const std::string&       ip,
                            const int                port,
                            ReadCallback             readcallback,
                            const int                id,
                            WriteCallback            writecallback )
{
    try
    {
        auto connection = create( io_context, readcallback, id, writecallback );
        boost::asio::ip::tcp::endpoint ep( boost::asio::ip::make_address( ip.c_str() ), port );
        connection->getSocket().connect( ep );
        connection->startReceive();
        LOG_INFO << "connected to " << ip << ":" << port << LOG_ENDL;
        return connection;
    }
    catch( boost::system::system_error& e )
    {
        LOG_ERROR << "connect to " << ip << ":" << port << "  failed. " << e.what() << LOG_ENDL;
        return nullptr;
    }
}

AsioTcp::~AsioTcp()
{
    try
    {
        disconnect();
    }
    catch( boost::system::system_error& e )
    {
        LOG_ERROR << "exceptions in disconnect. " << e.what() << LOG_ENDL;
    }
}

AsioTcp::Tcp::socket& AsioTcp::getSocket()
{
    return m_socket;
}

void AsioTcp::startReceive()
{
    auto self( shared_from_this() );
    m_socket.async_receive(
            boost::asio::buffer( m_recvBuf, 1024  ), [this, self]( const error_code& ec, size_t size ) {
                if(!m_readcallback( ec, m_recvBuf.data(), size, self ))
                {
                    LOG_ERROR << "tcp connection status changed: " << ec.message()
                              << ", ID: " << getId() << LOG_ENDL;
                    return;
                }
                startReceive();
            } );
}

void AsioTcp::disconnect()
{
    if( m_socket.is_open() )
    {
        m_socket.cancel();
        m_socket.shutdown( m_socket.shutdown_receive );
        std::this_thread::sleep_for( std::chrono::microseconds( 100 ) );
        m_socket.close();
    }
}

void AsioTcp::send( const void* data, size_t len )
{
    auto self( shared_from_this() );
    boost::asio::async_write( m_socket,
                              boost::asio::buffer( data, len ),
                              boost::bind( &AsioTcp::handle_write,
                                           self,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred,
                                           len ) );
}

AsioTcp::IpAddress AsioTcp::getLocalAddress() const
{
    return m_socket.local_endpoint().address();
}

AsioTcp::IpAddress AsioTcp::getRemoteAddress() const
{
    return m_socket.remote_endpoint().address();
}

const int AsioTcp::getId() const
{
    return m_id;
}

void AsioTcp::setId( int id )
{
    m_id = id;
}

void AsioTcp::setReadCallback( ReadCallback callback )
{
    m_readcallback = callback;
}

void AsioTcp::setWriteCallback( WriteCallback callback )
{
    m_writecallback = callback;
}

AsioTcp::AsioTcp( boost::asio::io_context& io_context,
                  ReadCallback&            readcallback,
                  WriteCallback&           writecallback,
                  int                      id )
        : m_id( id )
        , m_socket( io_context )
        , m_readcallback( readcallback ? readcallback : asioReadDefault )
        , m_writecallback( writecallback ? writecallback : asioWriteDefault )
{
}

void AsioTcp::handle_write( const error_code& ec, size_t sendlen, size_t origlen )
{
    m_writecallback( ec, sendlen, origlen, shared_from_this() );
    if( ec )
    {
        LOG_ERROR << m_id << " send failed, size to send: " << origlen << ", size send: " << sendlen
                  << ", error code " << ec.value() << ", msg: " << ec.message() << LOG_ENDL;
    }
}

AsioTcpServerPtr AsioTcpServer::create( IoServer&          io,
                                        const std::string& ip,
                                        const int          port,
                                        ReadCallBack       onRead,
                                        AcceptCallback     onAccept,
                                        const int          id,
                                        const bool         reuseAddr )
{
    try
    {
        AcceptorPtr acceptor( new Tcp::acceptor( io ) );

        error_code ec;
        auto       endpoint = Tcp::endpoint( boost::asio::ip::make_address( ip ), port );
        acceptor->open( endpoint.protocol(), ec );
        if( ec )
        {
            LOG_ERROR << "open socket failed! " << ec.message() << LOG_ENDL;
            return nullptr;
        }
        if( reuseAddr )
        {
            acceptor->set_option( boost::asio::socket_base::reuse_address( true ), ec );
            if( ec )
            {
                LOG_ERROR << "set socket reuse failed. " << ec.message() << LOG_ENDL;
                return nullptr;
            }
        }
        acceptor->bind( endpoint, ec );
        if( ec )
        {
            LOG_ERROR << "bind socket to " << ip << ":" << port << " failed. " << ec.message()
                      << LOG_ENDL;
            return nullptr;
        }
        acceptor->listen( LISTEN_BACKLOG, ec );
        if( ec )
        {
            LOG_ERROR << "listen to " << ip << ":" << port << " failed. " << ec.message()
                      << LOG_ENDL;
            return nullptr;
        }
        AsioTcpServerPtr connection{
                new AsioTcpServer( id, io, onRead, acceptor, onAccept, ip, port ) };
        connection->startAccept();
        return connection;
    }
    catch( const std::exception& e )
    {
        LOG_ERROR << "create tcp listener failed" << e.what() << LOG_ENDL;
        return nullptr;
    }
}

AsioTcpServer::AsioTcpServer( const int         id,
                              IoServer&         io,
                              ReadCallBack      onRead,
                              AcceptorPtr       acceptor,
                              AcceptCallback    onAccept,
                              const std::string ip,
                              const int         port )
        : m_id( id )
        , m_io( io )
        , m_onReadCallback( onRead ? onRead : asioReadDefault )
        , m_acceptor( acceptor )
        , m_onAcceptCallback( onAccept ? onAccept : []( AsioTcpPtr ) {} )
        , m_ip( ip )
        , m_port( port )
{
}

const int AsioTcpServer::getId() const
{
    return m_id;
}

void AsioTcpServer::setId( int id )
{
    m_id = id;
}

void AsioTcpServer::setOnAcceptCallback( AcceptCallback onAccept )
{
    m_onAcceptCallback = onAccept;
}

void AsioTcpServer::setOnReadCallback( ReadCallBack onRead )
{
    m_onReadCallback = onRead;
}

AsioTcpServer::IpAddress AsioTcpServer::getLocalAddress() const
{
    return m_acceptor->local_endpoint().address();
}

void AsioTcpServer::startAccept()
{
    using namespace boost::asio::ip;
    AsioTcpPtr new_connection = AsioTcp::create( m_io );
    m_acceptor->async_accept( new_connection->getSocket(),
                              boost::bind( &AsioTcpServer::handleAccept,
                                           this,
                                           new_connection,
                                           boost::asio::placeholders::error ) );
}

void AsioTcpServer::handleAccept( AsioTcpPtr new_connection, const error_code& error )
{
    if( !error )
    {
        LOG_INFO << "accepted from " << new_connection->getRemoteAddress().to_string() << LOG_ENDL;
        new_connection->setReadCallback( m_onReadCallback );
        m_onAcceptCallback( new_connection );
        new_connection->startReceive();
    }
    else
    {
        LOG_ERROR << "accept failed: " << error.message() << LOG_ENDL;
    }
    startAccept();
}

AsioTcpManager::AsioTcpManager( IoServer& ioContext )
        : m_io( ioContext )
{
}

AsioTcpManager::~AsioTcpManager()
{
}

int AsioTcpManager::addListen( const std::string ip,
                               const int         port,
                               AcceptCallback    onAccept,
                               ReadCallback      onRead )
{
    std::lock_guard<std::mutex> locker( m_lock );
    AsioTcpServerPtr            newServer{
            AsioTcpServer::create( m_io,
                                   ip,
                                   port,
                                   onRead ? onRead : asioReadDefault,
                                   std::bind(
                                           &AsioTcpManager::acceptNewConnection,
                                           this,
                                           onAccept ? onAccept : []( AsioTcpPtr& ) {},
                                           std::placeholders::_1 ),
                                   getNextConnectionId() ) };
    if( newServer == nullptr )
    {
        return -1;
    }
    m_listeners[newServer->getId()] = newServer;
    return newServer->getId();
}

void AsioTcpManager::acceptNewConnection( AcceptCallback acceptCallback, AsioTcpPtr connection )
{
    std::lock_guard<std::mutex> locker( m_lock );
    connection->setId( AsioTcpManager::getNextConnectionId() );
    m_connections[connection->getId()] = connection;
    acceptCallback( connection );
}

int AsioTcpManager::addTransmission( const std::string& ip,
                                     const int          port,
                                     ReadCallback       readCallback )
{
    try
    {
        std::lock_guard<std::mutex> locker( m_lock );

        AsioTcpPtr connection = AsioTcp::create( m_io, readCallback, getNextConnectionId() );
        if( connection == nullptr )
        {
            LOG_ERROR << "Create tcp connection failed." << LOG_ENDL;
            return -1;
        }
        Tcp::endpoint ep( boost::asio::ip::make_address( ip ), port );
        connection->getSocket().connect( ep );
        connection->startReceive();
        m_connections[connection->getId()] = connection;
        return connection->getId();
    }
    catch( const std::exception& e )
    {
        LOG_ERROR << "Add tcp connection failed. " << e.what() << LOG_ENDL;
        return -1;
    }
}

int AsioTcpManager::addTransmission( AsioTcpPtr& connection )
{
    if( connection == nullptr )
    {
        return -1;
    }
    std::lock_guard<std::mutex> locker( m_lock );
    if( m_connections.find( connection->getId() ) != m_connections.end() )
    {
        return -2;
    }
    if( connection->getId() < 0 )
    {
        connection->setId( getNextConnectionId() );
    }
    m_connections[connection->getId()] = connection;
    return connection->getId();
}

bool AsioTcpManager::removeConnection( AsioTcpPtr& connection )
{
    return removeConnection( connection->getId() );
}

bool AsioTcpManager::removeConnection( int id )
{
    std::lock_guard<std::mutex> locker( m_lock );
    if( m_connections.find( id ) != m_connections.end() )
    {
        m_connections.erase( id );
        return true;
    }
    else if( m_listeners.find( id ) != m_listeners.end() )
    {
        m_listeners.erase( id );
        return true;
    }
    else
    {
        return false;
    }
}

int AsioTcpManager::getNextConnectionId()
{
    int id;
    do
    {
        id = m_netxId.fetch_add( 1 );
    } while( m_connections.find( id ) != m_connections.end() ||
             m_listeners.find( id ) != m_listeners.end() );
    return id;
}

AsioTcpServerPtr AsioTcpManager::getServer( int id )
{
    auto tcp = m_listeners.find( id );
    return tcp == m_listeners.end() ? nullptr : tcp->second;
}

AsioTcpPtr AsioTcpManager::getClient( int id )
{
    auto tcp = m_connections.find( id );
    return tcp == m_connections.end() ? nullptr : tcp->second;
}

std::atomic_int AsioTcpManager::m_netxId( 0 );
