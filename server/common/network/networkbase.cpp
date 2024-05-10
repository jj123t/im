//
// Created by 25421 on 2024/4/24.
//

#include "networkbase.h"

#define LOG_INFO std::cout
#define LOG_ERROR std::cout
#define LOG_ENDL std::endl

NetworkBase::NetworkBase( const std::string& ip,
                          const int          port,
                          const NetworkType  type,
                          const bool         nonBlock,
                          const std::string  multicastInterface,
                          const std::string  sourceIP,
                          const int          id )
        : m_fd( -1 )
        , m_ip( ip )
        , m_port( port )
        , m_type( type )
        , m_nonBlock( nonBlock )
        , m_backlog( 128 )
        , m_accepted( false )
        , m_id( id )
        , m_sourceIP( sourceIP )
        , m_interface( multicastInterface )
{
    memset( &m_socketAddr, 0, sizeof( m_socketAddr ) );
    setup();
}

NetworkBase::NetworkBase( NetworkBase&& other )
        : m_fd( other.m_fd )
        , m_ip( other.m_ip )
        , m_port( other.m_port )
        , m_type( other.m_type )
        , m_nonBlock( other.m_nonBlock )
        , m_backlog( other.m_backlog )
        , m_accepted( other.m_accepted )
        , m_id( other.m_id )
        , m_sourceIP( other.m_sourceIP )
        , m_interface( other.m_interface )

{
    memcpy( &m_socketAddr, &other.m_socketAddr, sizeof( m_socketAddr ) );
    other.m_fd = -1;
    other.m_ip = "";
    other.m_port = -1;
    other.m_type = CLOSED;
    other.m_nonBlock = false;
    other.m_backlog = 128;
    other.m_accepted = false;

    other.m_sourceIP = "";
    other.m_interface = "";

    memset( &other.m_socketAddr, 0, sizeof( other.m_socketAddr ) );
}

NetworkBase::~NetworkBase()
{
    LOG_INFO << m_ip << ":" << m_port << ", " << m_fd << " diconnected." << LOG_ENDL;
    release( false );
}

NetworkBase::NetworkBase( int fd, NetworkType type, std::string ip, int port, int id )
        : m_fd( fd )
        , m_ip( ip )
        , m_port( port )
        , m_type( type )
        , m_nonBlock( false )
        , m_backlog( 128 )
        , m_accepted( true )
        , m_id( id )
{
    memset( &m_socketAddr, 0, sizeof( m_socketAddr ) );
}

NetworkBase& NetworkBase::operator=( NetworkBase&& other )
{
    m_fd = other.m_fd;
    m_ip = other.m_ip;
    m_port = other.m_port;
    m_type = other.m_type;
    m_id = other.m_id;

    other.m_fd = -1;
    other.m_ip = "";
    other.m_port = -1;
    m_type = CLOSED;
    return *this;
}

int NetworkBase::getFd() const
{
    return m_fd;
}

const std::string& NetworkBase::getIp() const
{
    return m_ip;
}

const int NetworkBase::getPort() const
{
    return m_port;
}

const struct sockaddr_in& NetworkBase::getSocketAddr() const
{
    return m_socketAddr;
}

const NetworkType NetworkBase::getType() const
{
    return m_type;
}

const std::string& NetworkBase::getMulticastSourceIP() const
{
    assert( m_type == MULTICASTRECEIVER || m_type == MULTICASTSENDER );
    return m_sourceIP;
}

const std::string& NetworkBase::getMulticastInterface() const
{
    assert( m_type == MULTICASTRECEIVER || m_type == MULTICASTSENDER );
    return m_interface;
}

bool NetworkBase::setup()
{
    bool success = false;
    switch( m_type )
    {
        case TCPTRANSMISSION:
        case UDPTRANSMISSION:
            success = create() && setSockaddrIn() && setNonBlock( m_nonBlock ) && connect();
            break;
        case TCPSERVER:
        case UDPSERVER:
            success = create() && setSockaddrIn() && setNonBlock( m_nonBlock ) && bind();
            break;
        case MULTICASTRECEIVER:
            success = create() && setSockaddrIn() && setNonBlock( m_nonBlock ) &&
                      joinMulticastGroup() && bind();
            break;
        case MULTICASTSENDER:
            success = create() && setSockaddrIn() && setNonBlock( m_nonBlock ) &&
                      joinMulticastGroup() && connect();
            break;
        default:
            break;
    }

//    LOG_INFO << "connecting to " << m_ip << ":" << m_port << LOG_ENDL;
    if( !success )
    {
        release( false );
        LOG_ERROR << "connect failed. " << m_ip << ":" << m_port << LOG_ENDL;
    }
    else
    {
        LOG_INFO << "connect success. " << m_ip << ":" << m_port << LOG_ENDL;
    }
    return success;
}

bool NetworkBase::create()
{
    int socktype = -1;
    switch( m_type )
    {
        case TCPTRANSMISSION:
        case TCPSERVER:
            socktype = SOCK_STREAM;
            break;
        case UDPTRANSMISSION:
        case UDPSERVER:
        case MULTICASTRECEIVER:
        case MULTICASTSENDER:
            socktype = SOCK_DGRAM;
            break;
        default:
#ifdef DEBUG
            LOG_ERROR << "socket type not supported." << LOG_ENDL;
#endif
            return false;
    }
    m_fd = ::socket( AF_INET, socktype, 0 );
    if( m_fd == -1 )
    {
#ifdef DEBUG
        LOG_ERROR << "create socket fd failed, error: " << ::strerror( errno )
                  << ". error no: " << errno << LOG_ENDL;
#endif
        return false;
    }
    if( socktype == SOCK_DGRAM )
    {
        if( !setReUseAddr() )
        {
#ifdef DEBUG
            LOG_ERROR << "set reuse address failed: " << ::strerror( errno )
                      << ". error no: " << LOG_ENDL;
#endif
            return false;
        }
    }
    return true;
}

bool NetworkBase::bind()
{
    if( ::bind( m_fd, (struct sockaddr*)&m_socketAddr, sizeof( m_socketAddr ) ) == -1 )
    {
#ifdef DEBUG
        LOG_ERROR << "bind socket error: " << ::strerror( errno ) << ". (errno: " << errno << ")"
                  << LOG_ENDL;
#endif
        return false;
    }
    return true;
}

bool NetworkBase::setNonBlock( bool nonblock )
{
    if( nonblock )
    {
        int flags = ::fcntl( m_fd, F_GETFL );
        if( ::fcntl( m_fd, F_SETFL, flags | O_NONBLOCK ) < 0 )
        {
            ::close( m_fd );
            m_fd = -1;
#ifdef DEBUG
            LOG_ERROR << "Set NONBLOCK failed" << LOG_ENDL;
#endif
            return false;
        }
    }
    else
    {
        int flags = ::fcntl( m_fd, F_GETFL, 0 );
        if( ::fcntl( m_fd, F_SETFL, flags & ~O_NONBLOCK ) < 0 )
        {
            ::close( m_fd );
            m_fd = -1;
#ifdef DEBUG
            LOG_ERROR << "Set NONBLOCK failed" << LOG_ENDL;
#endif
            return false;
        }
    }
    return true;
}

bool NetworkBase::setNoDelay( bool nodelay )
{
    int val = nodelay ? 1 : 0;
    int ret = 0;
    ret = ::setsockopt(
            m_fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>( &val ), sizeof( val ) );
    if( ret < 0 )
    {
#ifdef DEBUG
        LOG_ERROR << "client: " << m_fd << " set to TCP_NODELAY failed: " << ret << LOG_ENDL;
#endif
        return false;
    }
    return true;
}

bool NetworkBase::setSockaddrIn()
{
    ::memset( &m_socketAddr, 0, sizeof( m_socketAddr ) );
    m_socketAddr.sin_family = AF_INET;
    m_socketAddr.sin_port = htons( m_port );
    if( m_ip.empty() )
    {
        m_socketAddr.sin_addr.s_addr = htonl( INADDR_ANY );
    }
    else
    {
        if( ::inet_pton( AF_INET, m_ip.c_str(), &m_socketAddr.sin_addr ) <= 0 )
        {
#ifdef DEBUG
            LOG_ERROR << "inet_pton error for " << m_ip << LOG_ENDL;
#endif
            return false;
        }
    }
    return true;
}

bool NetworkBase::setReUseAddr()
{
    int reuse = 1;
    if( ::setsockopt( m_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) ) < 0 )
    {
        ::close( m_fd );
        m_fd = -1;
#ifdef DEBUG
        LOG_ERROR << "set udp reuseable failed" << errno << LOG_ENDL;
#endif
        return false;
    }
    return true;
}

bool NetworkBase::setLinger()
{
    struct linger lingerStruct;
    lingerStruct.l_onoff = 1;
    lingerStruct.l_linger = 0;
    return ::setsockopt(
            m_fd, SOL_SOCKET, SO_LINGER, (char*)&lingerStruct, sizeof( lingerStruct ) ) == 0;
}

bool NetworkBase::listen()
{
    assert( m_type == TCPSERVER );
    if( ::listen( m_fd, m_backlog ) == -1 )
    {
        return false;
    }
    return true;
}

bool NetworkBase::connect()
{
    if( ::connect( m_fd, (struct sockaddr*)&m_socketAddr, sizeof( m_socketAddr ) ) < 0 )
    {
#ifdef DEBUG
        LOG_ERROR << "connect error: " << ::strerror( errno ) << ". (errno: " << errno << ")"
                  << LOG_ENDL;
#endif
        return false;
    }
    return true;
}

bool NetworkBase::release( bool initiator )
{
    if( m_fd != -1 )
    {
#ifdef DEBUG
        LOG_INFO << "releasing socket " << m_fd << LOG_ENDL;
#endif
        if( m_type == TCPTRANSMISSION && !initiator )
        {
            std::array<char,1024> tmpbuf;
            ::shutdown( m_fd, SHUT_RD );
            usleep( 10 );
            ssize_t res = 0;
            do
            {
                res = recv( tmpbuf.data(), tmpbuf.size() );
            } while( res > 0 );
        }
        if( ::close( m_fd ) != 0 )
        {
#ifdef DEBUG
            LOG_ERROR << "socket got an abnormal shutdown " << m_fd << LOG_ENDL;
#endif
            ::shutdown( m_fd, SHUT_RDWR );
            m_fd = -1;
            return false;
        }
        else
        {
            m_fd = -1;
        }
    }
    return true;
}

bool NetworkBase::joinMulticastGroup()
{
    assert( m_type == MULTICASTRECEIVER || m_type == MULTICASTSENDER );
    if( m_interface.empty() )
    {
#ifdef DEBUG
        LOG_ERROR << "multicast needs interface ip, or you can just use udp socket." << LOG_ENDL;
#endif
        return false;
    }
    if( m_sourceIP.empty() )
    {
#ifdef DEBUG
        LOG_INFO << "IGMP v2" << LOG_ENDL;
#endif
        struct ip_mreq mreq;
        mreq.imr_multiaddr.s_addr = inet_addr( m_ip.c_str() );
        mreq.imr_interface.s_addr = inet_addr( m_interface.c_str() );
        int err = ::setsockopt( m_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof( mreq ) );
        if( err < 0 )
        {
#ifdef DEBUG
            LOG_ERROR << " ERR:setsockopt(): IP_ADD_MEMBERSHIP error: " << errno << LOG_ENDL;
#endif
            release( false );
            return false;
        }
    }
    else
    {
#ifdef DEBUG
        LOG_INFO << "IGMP v3" << LOG_ENDL;
#endif
        struct ip_mreq_source GroupInfo;
        ::memset( &GroupInfo, 0x00, sizeof( GroupInfo ) );
        GroupInfo.imr_multiaddr.s_addr = ::inet_addr( m_ip.c_str() );
        GroupInfo.imr_sourceaddr.s_addr = ::inet_addr( m_sourceIP.c_str() );
        GroupInfo.imr_interface.s_addr = ::inet_addr( m_interface.c_str() );
        if( ::setsockopt( m_fd,
                          IPPROTO_IP,
                          IP_ADD_SOURCE_MEMBERSHIP,
                          (char*)&GroupInfo,
                          sizeof( GroupInfo ) ) != 0 )
        {
#ifdef DEBUG
            LOG_ERROR << " setsockopt fail IP_ADD_SOURCE_MEMBERSHIP, " << m_ip << " " << m_sourceIP
                      << " " << m_interface << " " << LOG_ENDL;
#endif
            release( false );
            return false;
        }
    }
#ifdef DEBUG
    LOG_INFO << m_fd << ":Increment Add Membership:" << m_ip.c_str() << " port:" << m_port
             << LOG_ENDL;
#endif
    return true;
}

ssize_t NetworkBase::send( const char* data, size_t size )
{
//    int res = ::write(m_fd, data, size);
    ssize_t res = ::send( m_fd, data, size, 0 );
    if( res < 0 )
    {
#ifdef DEBUG
        LOG_ERROR << "send error: " << ::strerror( errno ) << ". (errno: " << errno << ")"
                  << LOG_ENDL;
#endif
    }
    return res;
}

void NetworkBase::setDelConnCallBack(std::function<void(NetworkBase*)> cb) {
    delConnCallback = cb;
}

ssize_t NetworkBase::recv( char* buf, size_t bufsize )
{
    ssize_t res = ::recv( m_fd, buf, bufsize, MSG_DONTWAIT );
    if( res < 0 )
    {
#ifdef DEBUG
        LOG_ERROR << "fd " << m_fd << " recv error: " << ::strerror( errno ) << ". (errno: " << errno << ")"
                  << LOG_ENDL;
#endif
        return -1;
    }
//    else if (res == 0) {
//        delConnCallback(this);
//    }
    return res;
}

ssize_t NetworkBase::sendTo( struct sockaddr_in& sendAddr, const void* data, size_t size )
{
    assert( m_type == UDPTRANSMISSION || m_type == UDPSERVER || m_type == MULTICASTSENDER );

    static socklen_t len = sizeof( sendAddr );

    ssize_t res = ::sendto( m_fd, data, size, 0, (struct sockaddr*)&sendAddr, len );
    return res;
}

ssize_t NetworkBase::recvFrom( struct sockaddr_in& recvAddr, char* buf, size_t bufsize )
{
    assert( m_type == UDPTRANSMISSION || m_type == UDPSERVER || m_type == MULTICASTRECEIVER );

    static socklen_t len = sizeof( recvAddr );

    ssize_t res = ::recvfrom( m_fd, buf, bufsize, 0, (struct sockaddr*)&recvAddr, &len );
    if( res < 0 || ( res == 0 && errno != EAGAIN ) )
    {
#ifdef DEBUG
        LOG_ERROR << "accept failed, error: " << ::strerror( errno ) << ". error no: " << errno
                  << LOG_ENDL;
#endif
    }
    return res;
}

int NetworkBase::accept()
{
    assert( m_type == TCPSERVER && !m_accepted );

    int res = ::accept( m_fd, (struct sockaddr*)NULL, NULL );
    if( res == -1 )
    {
#ifdef DEBUG
        LOG_ERROR << "accept failed, error: " << ::strerror( errno ) << ". error no: " << errno
                  << LOG_ENDL;
#endif
    }
    else
    {
#ifdef DEBUG
        LOG_INFO << "accepted " << res << LOG_ENDL;
#endif
    }

    return res;
}

int NetworkBase::accept( struct sockaddr_in& socketAddr )
{
    assert( m_type == TCPSERVER && !m_accepted );

    socklen_t addrSize = sizeof( sockaddr_in );

    int res = ::accept( m_fd, reinterpret_cast<sockaddr*>( &socketAddr ), &addrSize );
    if( res == -1 )
    {
#ifdef DEBUG
        LOG_ERROR << "accept failed, error: " << ::strerror( errno ) << ". error no: " << errno
                  << LOG_ENDL;
#endif
    }
    return res;
}

NetworkBasePtr NetworkBase::accept( int id )
{
    sockaddr_in socketAddr;
    int         clientFd = accept( socketAddr );
    if( clientFd > 0)
    {
        return std::make_shared<NetworkBase>( clientFd,
                                              TCPTRANSMISSION,
                                              std::string( inet_ntoa( socketAddr.sin_addr ) ),
                                              ntohs( socketAddr.sin_port ),
                                              id );
    }
    else
    {
        return nullptr;
    }
}

void NetworkBase::setSessionID( int id )
{
    m_id = id;
}

int NetworkBase::getID()
{
    return m_id;
}