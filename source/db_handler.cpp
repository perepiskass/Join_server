#include "db_handler.h"
#include <iostream>
#include <regex>
#include <memory>
#include <cstring>
#include <tuple>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct Point
{
  int x;
  int y;  
};

HandlerDB::HandlerDB(std::string name)
{
    if(fs::exists(fs::path(name))) std::cout << "DB exist" << std::endl;
    else 
    {
        std::ofstream temp(name,std::ios::ios_base::ate);
        std::cout << "DB created : " << name <<  std::endl;
        temp.close();
    }
    db_stream.open(name,std::ios::ios_base::in | std::ios::ios_base::app | std::ios::ios_base::binary);
    if(!db_stream.is_open()) std::cout << "Error open DB " << name << std::endl;
    else 
    {
        // Point p;
        // p.x = 77;
        // p.y = 99;
        // db_stream.read((char*)&p,sizeof(Point));
        // std::cout << p.x << ' ' << p.y;
    }
}
HandlerDB::~HandlerDB()
{
    db_stream.close();
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
        const char delim = ' ';
        std::string::size_type start = str.find_first_not_of("INSERT ");
        std::string::size_type end = 0;
        start = str.find_first_not_of(delim,start);
        end = str.find_first_of(delim,start);
        char table = str.substr(start,end-start)[0];
        start = end;
        start = str.find_first_not_of(delim,start);
        end = str.find_first_of(delim,start);
        size_t id =atoi(str.substr(start,end-start).c_str());
        start = end;
        start = str.find_first_not_of(delim,start);
        end = str.find_first_of(delim,start);
        char name [11];
        strcpy(name,str.substr(start,end-start).c_str());
        
        std::unique_ptr<Handler_c> hanlde_c {new Insert(db_stream,table,id,name)};
        response = hanlde_c->handle_command();
    }
    else  if (std::regex_match(str,std::regex ("TRANCATE+ +[A-Z]")))
    {
        std::regex table_reg{ " +[A-Z]" };
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


    else response = "Bad command";
}