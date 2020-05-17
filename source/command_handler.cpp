#include "command_handler.h"
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

    Handler_c::Handler_c(){}

    std::fstream Handler_c::openDB(std::string name_db, std::ios::ios_base::openmode mode)
    {
       std::fstream db_str (name_db, mode);
        if(db_str.is_open())
        {
            return db_str;
        }
        else
        {
            std::cout << "Error open db name: " << name_db << std::endl;
        }
    }



    Insert::Insert(std::tuple<std::string,char, size_t, std::string> args_): args(args_)
    {}
    std::string Insert::handle_command()
    {
        Table tab;
        std::tie(std::ignore, tab.table_name,tab.id,tab.name) = args;
        auto db_str = openDB(std::get<0>(args), std::ios::ios_base::app | std::ios::ios_base::binary);
        db_str.write((char*)&tab,sizeof(Table));
        db_str.close();
        return "OK";
    }

    Trancate::Trancate(std::tuple<std::string,char> args_): args(args_)
    {}
    std::string Trancate::handle_command()
    {
        Table tab;
        char name_table;
        std::string name_db;
        std::tie(name_db,name_table) = args;

        auto db_str = openDB(name_db, std::ios::ios_base::in | std::ios::ios_base::binary);
        auto db_str_copy = openDB(name_db+".copy", std::ios_base::app | std::ios_base::trunc | std::ios::ios_base::binary);

        while( db_str.read((char*)&tab,sizeof(Table)))
        {
            if(tab.table_name == name_table) continue;
            else
            {
                db_str_copy.write((char*)&tab,sizeof(Table));
            }
        }
        db_str.close();
        db_str_copy.close();

        if (fs::remove(fs::path(name_db))) fs::rename(fs::path(name_db+"copy"),fs::path(name_db));
        return "OK";
    }


    Intersection::Intersection(std::tuple<std::string> args_): args(args_)
    {}
    std::string Intersection::handle_command()
    {
        Table tab;
        char name_table;
        std::string name_db;
        std::tie(name_db) = args; // добавить название таблиц

        auto db_str = openDB(name_db, std::ios::ios_base::in | std::ios::ios_base::binary);
    }

    Symmetric_difference::Symmetric_difference(std::tuple<std::string> args_): args(args_)
    {}
    std::string handle_command()
    {
        
    }


