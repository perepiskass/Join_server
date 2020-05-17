#pragma once
#include <string>
#include <tuple>
#include <fstream>

struct Table
{    
    char  table_name;
    size_t id;
    char name[10];
};

class Handler_c
{
    public:
        Handler_c();
        virtual std::string handle_command() = 0;
        virtual ~Handler_c()=default;
        std::fstream openDB(std::string name_db, std::ios::ios_base::openmode mode);
};

class Insert: public Handler_c
{   
    public:
        Insert(std::tuple<std::string, char, size_t, std::string> args_);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Insert()=default;

    private:
        std::tuple<std::string,char, size_t, std::string> args;
};

class Trancate: public Handler_c
{
    public:
        Trancate(std::tuple<std::string, char> args_); //(std::tuple<std::string,char> args);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Trancate()=default;

    private:
        std::tuple<std::string,char> args;
};

class Intersection: public Handler_c
{
    public:
        Intersection(std::tuple<std::string> args);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Intersection()=default;

    private:
        std::tuple<std::string> args;
};

class Symmetric_difference: public Handler_c
{
    public:
        Symmetric_difference(std::tuple<std::string> args);
        std::string handle_command() override;
        // std::fstream& openDB(std::string name_db) override;
        ~Symmetric_difference()=default;

    private:
        std::tuple<std::string> args;
};

