#pragma once
#include "command_handler.h"
#include <memory>

class HandlerDB
{
    public:
        HandlerDB(std::string name);
        std::string setCommand(std::string command);
        ~HandlerDB();

    private:
        void checkCommand(std::string);
        std::string db_name;
        std::fstream db_stream;
        std::string response;
};
