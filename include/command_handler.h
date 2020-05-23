#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <map>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

/**
 * @brief Структура, определяет вид в каком данные храняться в базе данных.
 */
struct Table
{    
    char  table_name;
    u_int32_t id = 0;
    char name[16]{0};
};

/**
 * @brief Интерфейс для различных классов - команд в базу данных
 */
class Handler_c
{
    public:
        Handler_c(std::fstream& db_stream);
        virtual std::string handle_command()const = 0;
        virtual ~Handler_c()=default;
        std::vector<Table> readAll()const;
        std::string makeResponse(std::map<u_int32_t,std::pair<std::string,std::string>>& result)const;
        
        std::fstream& db_stream;
};

/**
 * @brief Класс, реализующий команду вставки строки в одну из табиц базы данных.
 */
class Insert: public Handler_c
{   
    public:
        Insert(std::fstream& db_stream,const char name_table,u_int32_t ID,const char* name);
        std::string handle_command()const override;
        ~Insert()=default;

    private:
        Table insert_tab;
};

/**
 * @brief Класс, реализующий команду удаления данных одной из табиц.
 */
class Trancate: public Handler_c
{
    public:
        Trancate(std::fstream& db_stream,const char name_table);
        std::string handle_command()const override;
        ~Trancate()=default;

    private:
        const char table_name;
};

/**
 * @brief Класс, реализующий команду выборки данных из двух таблиц, с одинаковыми полями ID.
 */
class Intersection: public Handler_c
{
    public:
        Intersection(std::fstream& db_stream);
        std::string handle_command()const override;
        ~Intersection()=default;
};

/**
 * @brief Класс, реализующий команду выборки данных из двух таблиц, с не пересекающимися ID.
 */
class Symmetric_difference: public Handler_c
{
    public:
        Symmetric_difference(std::fstream& db_stream);
        std::string handle_command()const override;
        ~Symmetric_difference()=default;
};

/**
 * @brief Класс, реализующий команду дефрагминтации файла базы данных.
 */
class Defragmintation_db: public Handler_c
{
    public:
        Defragmintation_db(std::fstream& db_stream,const std::string& db_name);
        std::string handle_command()const override;
        ~Defragmintation_db()=default;
    private:
        const std::string db_name;
};

