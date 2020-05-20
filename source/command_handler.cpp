#include "command_handler.h"
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <utility>
#include <queue>

namespace fs = boost::filesystem;

    void printAll(std::vector<Table>& t)
    {
        for (auto& i : t)
        {
            std::cout<< i.table_name << ' ' << i.id << ' ' << i.name << std::endl;
        }
    }
    Handler_c::Handler_c(std::fstream& db_stream): db_stream(db_stream)
    {}

    std::vector<Table> Handler_c::readAll()
    {
        // std::cout << "read All start" << std::endl;
        // char ss[16]{"BEFORE readAll\0"};
        // db_stream.seekp(0,db_stream.end);
        // db_stream.write((char*)&ss,16);
        // std::cout << "readAll end\n";

        // db_stream.seekg(0,db_stream.beg);
        // char st[40];
        // db_stream.read((char*)&st,40);
        // std::cout << st << std::endl;
        // db_stream.clear();

        db_stream.seekg(0,db_stream.beg);
        std::vector<Table> result;
        Table tab;
        while( db_stream.read((char*)&tab,sizeof(Table)))
        {
            result.emplace_back(tab);
        }
        // printAll(result);
        // char s[14]{"FROM readAll\0"};
        // db_stream.seekp(0,db_stream.end);
        // db_stream.write((char*)&s,14);
        // std::cout << "readAll end\n";
        db_stream.clear();
        return result;
    }

    Insert::Insert(std::fstream& db_s,char name_table,size_t ID,char* name):Handler_c(db_s)
    {
        insert_tab.table_name = name_table;
        insert_tab.id = ID;
        strcpy(insert_tab.name,name);
    }
    std::string Insert::handle_command()
    {   
        auto all_data = readAll();
        // db_stream << "after all_data_readALL";

        // printAll(all_data);
        auto name_tab = insert_tab.table_name;
        auto ID = insert_tab.id;
        
        auto it = std::find_if(std::begin(all_data),std::end(all_data),[&ID,&name_tab](Table& tab){
            std::cout << "tab.id " << tab.id << " ID " << ID << std::endl;
            if(tab.id == ID && tab.table_name==name_tab)
            {
                std::cout << "return true \n";
                return true;
            } 
            else 
            {
                std::cout << "return false \n";
                return false;
            }
        });
        if (it == all_data.end())
        {
            std::cout << "open for write table" << std::endl;
            db_stream.seekp(0,std::ios::end);
            if(db_stream.is_open())
            {
                std::cout <<sizeof(Table) << ' ' << insert_tab.table_name << ' ' << insert_tab.id << ' ' << insert_tab.name << std::endl;
                db_stream.write((char*)&insert_tab,sizeof(Table));
                // db_stream << "FUck";
                std::cout << "db writed!!!\n";
            }
            else
            {
                std::cout << "Error open file\n";
            }
            db_stream.clear();
            return "OK";
        }
        else return "This element exist!";
    }

    Trancate::Trancate(std::fstream& db_s,char name_table): Handler_c(db_s), table_name(name_table)
    {}
    std::string Trancate::handle_command()
    {
        auto all_data = readAll();

        // db_stream.seekg (0,std::ios::end);
        // auto length_all = db_stream.tellg();
        // db_stream.seekg (0,std::ios::beg);
        // auto length = length_all - db_stream.tellg();
        // db_stream.clear();
        // std::cout << "all data readed\n";
        // db_stream.seekp(std::ios::beg);
        // const char zero = '\0';
        // db_stream.write((char*)&zero,length);
        // db_stream.clear();
        

        std::cout << "all data cleared\n";

        db_stream.seekp(std::ios::beg);
        for(auto& tab : all_data)
        {
            if(tab.table_name != this->table_name)
            {
                std::cout << "Before write\n";
                db_stream.write((char*)&tab,sizeof(Table));
                std::cout << "After write\n";

            }
        }
        db_stream.clear();     
        return "OK";
    }

    Intersection::Intersection(std::fstream& db_s): Handler_c(db_s)
    {}
    std::string Intersection::handle_command()
    {
        auto all_data = readAll();
        std::map<size_t,std::pair<std::string,std::string>> result;

        std::for_each(all_data.begin(), all_data.end(), [&all_data,&result](const Table& tab){ 
        if(tab.table_name == 'A')
        {
                auto it = std::find_if(std::begin(all_data),std::end(all_data),[&tab](const Table& tab_){
                    if(tab_.id == tab.id && tab_.table_name=='B') return true;
                    else return false;
                });
            if(it!= all_data.end())
            {
                 result[tab.id] = std::make_pair(tab.name,it->name);
            }
        }
        });
        std::string response;
        for(auto& res : result)
        {
            response+= std::to_string(res.first)+", " + res.second.first+", " + res.second.second + '\n';
        }
        response+="OK";
        return response;
    }

    Symmetric_difference::Symmetric_difference(std::fstream& db_s): Handler_c(db_s)
    {}
    std::string Symmetric_difference::handle_command()
    {
        auto all_data = readAll();
        std::map<size_t,std::pair<std::string,std::string>> result;

        std::for_each(all_data.begin(), all_data.end(), [&all_data,&result](const Table& tab){ 

            auto it = std::find_if(std::begin(all_data),std::end(all_data),[&tab](const Table& tab_){
                if(tab_.id == tab.id && tab_.table_name!=tab.table_name) return true;
                else return false;
            });
        if(it== all_data.end())
        {
            if(tab.table_name == 'A') result[tab.id] = std::make_pair(tab.name,"");
            else result[tab.id] = std::make_pair("",tab.name);
        }
        
        });
        std::string response;
        for(auto& res : result)
        {
            response+=std::to_string(res.first) +", " + res.second.first+", " + res.second.second + '\n';
        }
        response+="OK";
        return response;
    }


