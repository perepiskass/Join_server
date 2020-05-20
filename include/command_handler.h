#pragma once
#include <string>
#include <fstream>
#include <map>
#include <vector>


struct Table
{    
    char  table_name;
    size_t id;
    char name[11];
};

class Handler_c
{
    public:
        Handler_c(std::fstream& db_stream);
        virtual std::string handle_command() = 0;
        virtual ~Handler_c()=default;
        std::vector<Table> readAll();
        
        std::fstream& db_stream;
};

class Insert: public Handler_c
{   
    public:
        Insert(std::fstream& db_stream,char name_table,size_t ID,char* name);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Insert()=default;

    private:
        Table insert_tab;
};

class Trancate: public Handler_c
{
    public:
        Trancate(std::fstream& db_stream,char name_table); //(std::tuple<std::string,char> args);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Trancate()=default;

    private:
        const char table_name;
};

class Intersection: public Handler_c
{
    public:
        Intersection(std::fstream& db_stream);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Intersection()=default;
};

class Symmetric_difference: public Handler_c
{
    public:
        Symmetric_difference(std::fstream& db_stream);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Symmetric_difference()=default;
};

