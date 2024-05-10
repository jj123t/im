//
// Created by 25421 on 2024/5/1.
//

#include "asiotimer.h"
#include <boost/make_shared.hpp>
#include <iostream>

#define LOG_ERROR std::cout
#define LOG_INFO std::cout
#define LOG_ENDL std::endl

AsioTimerPtr AsioTimer::create( boost::asio::io_context& io_context,
                                int                      waitmillsecs,
                                TimerCallback            callback,
                                const int                id )
{
    auto timer = AsioTimerPtr( new AsioTimer( io_context, waitmillsecs, callback, id ) );
    try
    {
        std::cout << "timer start ok !!!\n";
        timer->start();
        return timer;
    }
    catch( boost::system::system_error& err )
    {
        LOG_ERROR << "expires_from_now failed. " << err.what() << LOG_ENDL;
        return nullptr;
    }
}

void AsioTimer::start()
{
    m_timer.expires_from_now(
            boost::posix_time::milliseconds( m_waitmillsecs ) ); // may throw exception

    auto self( shared_from_this() );
    m_timer.async_wait( [this, self]( const boost::system::error_code& ec ) {
        if( m_callback( ec, self ) )
        {
            start();
        }
        else
        {
            LOG_ERROR << "timer callback return false. ID: " << getId() << LOG_ENDL;
        }
    } );
}

void AsioTimer::cancel()
{
    boost::system::error_code ec;
    m_timer.cancel( ec );
}

void AsioTimer::setId( int id )
{
    m_id = id;
}

const int AsioTimer::getId() const
{
    return m_id;
}

void AsioTimer::setReadCallback( TimerCallback callback )
{
    m_callback = callback;
}

void AsioTimer::setIntervalMillsec( int millsecs )
{
    m_waitmillsecs = millsecs;
}

AsioTimer::AsioTimer( boost::asio::io_context& io_context,
                      int                      waitmillsecs,
                      TimerCallback            callback,
                      int                      id )
        : m_id( id )
        , m_waitmillsecs( waitmillsecs )
        , m_timer( io_context )
        , m_callback(callback )
{
}
