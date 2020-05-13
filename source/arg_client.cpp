#include "version_lib.h"
#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

/**
 * @brief Функция для проверки введенных аргументов программы ip адреса сервера и номера порта для подключения.
 */
auto checkArg(int argc,char** argv)
{
    if ( argc > 1)
    {
        std::string version = "-version";
        if (argv[1] == version)
        {
            std::cout << "version: " << version_major()<< '.'<< version_minor() << '.' << version_patch() << std::endl;
            exit(0);
        }
        else
        {
            std::pair<boost::asio::ip::address,size_t> result;
            boost::system::error_code ec;
            result.first = boost::asio::ip::address::from_string(argv[1], ec );
            result.second = atoi(argv[2]);
            if (!ec && result.second > 0) 
            {
                return result;
            } 
            else  
            {

                std::cout << "Enter correct address or port"<< std::endl;
                exit(0);
            }
        }
    }
    else 
    {
      std::cout << "Usage: bulk_client <ip_address> <port> or -version" << std::endl;
      exit(0);
    }
    
}