#include "db_handler.h"
#include "command_handler.h"
#include <regex>
#include <memory>
#include <cstring>

namespace fs = boost::filesystem;

HandlerDB::HandlerDB(const std::string& name): db_name(name)
{
    if(fs::exists(fs::path(name))) std::cout << "DB exist" << std::endl;
    else 
    {
        std::ofstream temp(name,std::ios::ios_base::ate);
        std::cout << "DB created : " << name << std::endl;
        temp.close();
    }
    db_stream.open(name,std::ios::ios_base::in | std::ios::ios_base::out | std::ios::ios_base::binary);
    if(!db_stream.is_open()) std::cout << "Error open DB " << name << std::endl;
}
HandlerDB::~HandlerDB()
{
    db_stream.close();
}

std::string HandlerDB::setCommand(const std::string& command)
{
    checkCommand(command);
    return response;
}

void HandlerDB::checkCommand(const std::string& str)
{
    if (std::regex_match(str,std::regex ("INSERT+ +[A-B]+ +[0-9]{1,9}+ +[a-z|A-Z]{1,15}")))
    {
        const char delim = ' ';
        std::string::size_type start = str.find_first_not_of("INSERT ");
        std::string::size_type end = 0;
        start = str.find_first_not_of(delim,start);
        end = str.find_first_of(delim,start);
        char table = str.substr(start,end-start)[0];
        start = end;
        start = str.find_first_not_of(delim,start);
        end = str.find_first_of(delim,start);
        u_int32_t id =atoi(str.substr(start,end-start).c_str());
        start = end;
        start = str.find_first_not_of(delim,start);
        end = str.find_first_of(delim,start);
        char name [16];
        strcpy(name,str.substr(start,end-start).c_str());
        
        std::unique_ptr<Handler_c> hanlde_c {new Insert(db_stream,table,id,name)};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("TRANCATE+ +[A-B]")))
    {
        std::regex table_reg{ " +[A-B]" };
        std::sregex_iterator beg{ str.cbegin(), str.cend(), table_reg }; 
        char table = beg->str()[1];

        std::unique_ptr<Handler_c> hanlde_c {new Trancate(db_stream,table)};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("INTERSECTION")))
    {
        std::unique_ptr<Handler_c> hanlde_c {new Intersection(db_stream)};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("SYMMETRIC_DIFFERENCE")))
    {
        std::unique_ptr<Handler_c> hanlde_c {new Symmetric_difference(db_stream)};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("DEFRAGMINTATION_DB")))
    {
        std::unique_ptr<Handler_c> hanlde_c {new Defragmintation_db(db_stream,db_name)};
        response = hanlde_c->handle_command();
    }

    else response = "ERR message";
}