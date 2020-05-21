#include "version_lib.h"
#include <iostream>
#include <string>
#include <utility>

/**
 * @brief Функция для проверки введенных аргументов программы порта и размера bulk.
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
            std::pair<size_t,std::string> result;
            result.first = atoi(argv[1]);
            result.second = argv[2];
            if (result.first > 0 && !result.second.empty()) 
            {
                return result;
            } 
            else  
            {
                std::cout << "Enter correct port or data base name"<< std::endl;
                exit(1);
            }
        }
    }
    else 
    {
      std::cout << "Usage: async_tcp_echo_server <port> <db name> or -version" << std::endl;
      exit(1);
    }
    
}