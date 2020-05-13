#include "version_lib.h"
#include "asio_async_server.h"
#include <iostream>


std::pair<size_t,std::string> checkArg(int argc,char** argv);
server* serverPtr;

void signalFunction(const boost::system::error_code & err, int signal)
{
  if(!err)
  {
    std::cout << std::endl;
    if (signal == 2)
    {
      serverPtr->~server();
      delete serverPtr;
      exit(0);
    }
  }
  exit(1);
}

int main(int argc, char *argv[]) 
{
  try
  {
    auto args = checkArg(argc,argv);

    boost::asio::io_service io_service;

    boost::asio::signal_set sig(io_service, SIGINT, SIGTERM);
    sig.async_wait(signalFunction);
    serverPtr = new server(io_service, args);
    io_service.run();
  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
