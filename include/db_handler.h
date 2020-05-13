#pragma once
#include <string>
#include <thread>

class HandlerDB
{
    public:
        HandlerDB(std::string name);
        std::string setCommand(std::string command);

    private:
        std::string name_db;
    
};
