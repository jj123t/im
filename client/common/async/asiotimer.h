//
// Created by 25421 on 2024/5/1.
//

#ifndef TESTMYSQL_ASIOTIMER_H
#define TESTMYSQL_ASIOTIMER_H


#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

class AsioTimer;
using AsioTimerPtr = boost::shared_ptr<AsioTimer>;
class AsioTimer : public boost::enable_shared_from_this<AsioTimer>
{
public:
    using TimerCallback = std::function<bool( const boost::system::error_code&, AsioTimerPtr )>;
    static AsioTimerPtr create(
            boost::asio::io_context& io_context,
            int                      waitmillsecs,
            TimerCallback            callback = []( const boost::system::error_code& ec,
                                                    AsioTimerPtr ) { return true; },
            const int                id = -1 );
    void start();
    void cancel();
    void setId( int id );
    const int getId() const;
    void setReadCallback( TimerCallback callback );
    void setIntervalMillsec( int millsecs );
private:
    AsioTimer( boost::asio::io_context& io_context,
               int                      waitmillsecs,
               TimerCallback            callback,
               int                      id );
private:
    int                         m_id;           // 定时器的ID
    int                         m_waitmillsecs; // 定时器的时间间隔
    boost::asio::deadline_timer m_timer;        // asio的deadline_timer
    TimerCallback               m_callback;     // 回调函数
};


#endif //TESTMYSQL_ASIOTIMER_H
