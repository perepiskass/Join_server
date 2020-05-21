#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <map>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct Table
{    
    char  table_name;
    size_t id = 0;
    char name[11]{0};
};

class Handler_c
{
    public:
        Handler_c(std::fstream& db_stream);
        virtual std::string handle_command()const = 0;
        virtual ~Handler_c()=default;
        std::vector<Table> readAll()const;
        std::string makeResponse(std::map<size_t,std::pair<std::string,std::string>>& result)const;
        
        std::fstream& db_stream;
};

class Insert: public Handler_c
{   
    public:
        Insert(std::fstream& db_stream,char name_table,size_t ID,char* name);
        std::string handle_command()const override;
        ~Insert()=default;

    private:
        Table insert_tab;
};

class Trancate: public Handler_c
{
    public:
        Trancate(std::fstream& db_stream,char name_table);
        std::string handle_command()const override;
        ~Trancate()=default;

    private:
        const char table_name;
};

class Intersection: public Handler_c
{
    public:
        Intersection(std::fstream& db_stream);
        std::string handle_command()const override;
        ~Intersection()=default;
};

class Symmetric_difference: public Handler_c
{
    public:
        Symmetric_difference(std::fstream& db_stream);
        std::string handle_command()const override;
        ~Symmetric_difference()=default;
};

class Defragmintation_db: public Handler_c
{
    public:
        Defragmintation_db(std::fstream& db_stream, std::string db_name);
        std::string handle_command()const override;
        ~Defragmintation_db()=default;
    private:
        std::string db_name;
};

