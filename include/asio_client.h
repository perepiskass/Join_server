#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace ba = boost::asio;

class client
{
    public:
        client();
        void connect(ba::ip::address& addres, size_t port);
        void disconnect();
        void write(std::string&& str);
        void read();
    private:
        ba::io_service io_context;
        ba::ip::tcp::endpoint ep;
        ba::ip::tcp::socket socket_;
};
    

