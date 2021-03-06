#include "asio_async_server.h"
#include <iostream>

size_t session::count_connections = 0;


//-----session-----------------------------------------------------------
session::session(tcp::socket socket,std::shared_ptr<HandlerDB> handler_): socket_(std::move(socket)), handler_db(handler_)
  {
    session::count_connections++;
  }

void session::start()
{
  do_read();
}

void session::do_read()
{
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(data_,max_length),
      [this, self](boost::system::error_code ec, std::size_t)
      {
        if ( ec!=boost::asio::error::eof )
        {
          auto data_str =  handler_db->setCommand(std::string(data_));
          memset(data_,0,sizeof(data_));
          do_write(data_str);
        }
        else 
        {
          --count_connections;
          if(!count_connections)
          {
            std::cout << "all connections closed" << std::endl;
          } 
        }
      });

}
void session::do_write(std::string data_str)
{
  auto self(shared_from_this());
  boost::asio::async_write(socket_, boost::asio::buffer(data_str, data_str.length()),
      [this, self](boost::system::error_code ec, std::size_t /*length*/)
      {
        if (!ec)
        {
          do_read();
        }
      });
}

//-----server-----------------------------------------------------------
server::server(boost::asio::io_service& io_service, std::pair<size_t,std::string>& args)
  : acceptor_(io_service, tcp::endpoint(tcp::v4(), args.first)),
    socket_(io_service),handler_db(new HandlerDB(args.second))
{
  do_accept(handler_db);
}

server::~server()
{
  acceptor_.close();
}

void server::do_accept(std::shared_ptr<HandlerDB> handler_db)
{
  acceptor_.async_accept(socket_,
      [this,handler_db](boost::system::error_code ec)
      {
        if (!ec)
        {
          std::make_shared<session>(std::move(socket_),handler_db)->start();
        }
        do_accept(handler_db);
      });
}


