#pragma once
#include <string>
#include <thread>
#include "command_handler.h"

class HandlerDB
{
    public:
        HandlerDB(std::string name);
        std::string setCommand(std::string command);

    private:
        void checkCommand(std::string);
        
        // Handler_c* hanlde_c;
        std::string response;
        std::string name_db;
    
};
