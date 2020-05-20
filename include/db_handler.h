#pragma once
#include <string>
#include <thread>
#include "command_handler.h"
#include <fstream>

class HandlerDB
{
    public:
        HandlerDB(std::string name);
        std::string setCommand(std::string command);
        ~HandlerDB();

    private:
        void checkCommand(std::string);
        
        std::fstream db_stream;
        std::string response;
    
};
