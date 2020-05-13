#include "db_handler.h"
#include <iostream>

HandlerDB::HandlerDB(std::string name): name_db(name)
{
    std::cout << name_db << std::endl;
}

std::string HandlerDB::setCommand(std::string command)
{
    if (command == "wait")
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        return command;
    }
    else 
    {
        std::cout << command << ' ' << name_db << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return name_db;
    }
}