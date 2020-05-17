#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include "db_handler.h"
#include <string>

using boost::asio::ip::tcp;

class session: public std::enable_shared_from_this<session>
{
  public:
    session(tcp::socket socket,std::shared_ptr<HandlerDB> handler_db);
    void start();

  private:
    void do_read();
    void do_write(std::string);

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[1024];
    int offset = 0;

    // std::string data_str;
    std::shared_ptr<HandlerDB> handler_db;
    static size_t count;
};

class server
{
  public:
    server(boost::asio::io_service& io_service, std::pair<size_t,std::string>& args);
    ~server();
  private:
    void do_accept(std::shared_ptr<HandlerDB> handler_db);

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::shared_ptr<HandlerDB> handler_db;
};
