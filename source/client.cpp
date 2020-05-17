#include "version_lib.h"
#include "asio_client.h"
#include <iostream>
#include <string>
#include <csignal>

std::pair<ba::ip::address,size_t> checkArg(int argc,char** argv);

client* clientPtr;

void signalFunction(int signal)
{
  std::cout << std::endl;
  if (signal == 2)
  {
    clientPtr->disconnect();
    delete clientPtr;
    exit(0);
  }
  exit(1);
}


int main(int argc, char *argv[]) 
{
  auto args = checkArg(argc,argv);
  try
  {
    clientPtr = new client{};
    clientPtr->connect(args.first,args.second);

    signal(SIGINT,signalFunction);

    std::string str;
    while(true)
    {
      std::getline(std::cin,str);
      clientPtr->write(std::move(str));
    }
  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
