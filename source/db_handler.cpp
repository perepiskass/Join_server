#include "db_handler.h"
#include <iostream>
#include <regex>
#include <memory>

HandlerDB::HandlerDB(std::string name): name_db(name)
{

}

std::string HandlerDB::setCommand(std::string command)
{
    checkCommand(command);
    return response;
}

void HandlerDB::checkCommand(std::string str)
{
    if (std::regex_match(str,std::regex ("INSERT+ +[A-Z]+ +[0-9]{1,3}+ +[a-z|A-Z]{1,10}")))
    {
        std::regex table_reg{ " +[A-Z]+ " };
        std::sregex_iterator beg{ str.cbegin(), str.cend(), table_reg }; 
        char table = beg->str()[1];
        std::regex id_reg{ "[0-9]{1,3}" };
        std::sregex_iterator beg{ str.cbegin(), str.cend(), id_reg };
        size_t id = atoi(beg->str().c_str());
        std::regex name_reg{ " +[a-z|A-Z]{1,10}" };
        std::sregex_iterator beg{ str.cbegin(), str.cend(), id_reg };
        auto name = beg->str();   ///убрать пробел
        
        std::unique_ptr<Handler_c> hanlde_c {new Insert(std::make_tuple(name_db,table,id,name))};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("TRANCATE+ +[A-Z]")))
    {
        std::regex table_reg{ " +[A-Z]" };
        std::sregex_iterator beg{ str.cbegin(), str.cend(), table_reg }; 
        char table = beg->str()[1];

        std::unique_ptr<Handler_c> hanlde_c {new Trancate(std::make_tuple(name_db,table))};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("INTERSECTION")))
    {
        std::unique_ptr<Handler_c> hanlde_c {new Intersection(std::make_tuple(name_db))};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("SYMMETRIC_DIFFERENCE")))
    {
        std::unique_ptr<Handler_c> hanlde_c {new Symmetric_difference(std::make_tuple(name_db))};
        response = hanlde_c->handle_command();
    }


    else response = "Bad command";
}