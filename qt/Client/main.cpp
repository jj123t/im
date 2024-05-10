#include "src/client.h"
#include "src/login.h"
#include <QApplication>
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    // boost::asio::io_context io;
    // boost::asio::deadline_timer t(io);
    // // boost::timer t = new boost::timer();
    QApplication a(argc, argv);
    Client w;
    return a.exec();
}
