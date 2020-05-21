#include "command_handler.h"
#include <set>

    Handler_c::Handler_c(std::fstream& db_stream): db_stream(db_stream)
    {}

    std::vector<Table> Handler_c::readAll()const
    {
        db_stream.seekg(0,std::ios::beg);
        std::vector<Table> result;
        Table tab;
        while( db_stream.read((char*)&tab,sizeof(Table)))
        {
            result.emplace_back(tab);
        }
        db_stream.clear();
        return result;
    }

    std::string Handler_c::makeResponse(std::map<size_t,std::pair<std::string,std::string>>& result)const
    {
        std::string response;
        for(auto& res : result)
        {
            response+=std::to_string(res.first) +"," + res.second.first+"," + res.second.second + '\n';
        }
        response+="OK";
        return response;
    }

    Insert::Insert(std::fstream& db_s,char name_table,size_t ID,char* name):Handler_c(db_s)
    {
        insert_tab.table_name = name_table;
        insert_tab.id = ID;
        strcpy(insert_tab.name,name);
    }
    std::string Insert::handle_command()const
    {   
        auto all_data = readAll();

        auto name_tab = insert_tab.table_name;
        auto ID = insert_tab.id;
        
        auto it = std::find_if(std::begin(all_data),std::end(all_data),[&ID,&name_tab](Table& tab){
            if(tab.id == ID && tab.table_name==name_tab) return true;
            else return false;
        });
        if (it == all_data.end())
        {
            db_stream.seekp(0,std::ios::end);
            if(db_stream.is_open())
            {
                db_stream.write((char*)&insert_tab,sizeof(Table));
            }
            else
            {
                std::cout << "Error open file\n";
            }
            db_stream.clear();
            return "OK";
        }
        else return "ERR duplicate " + std::to_string(ID);
    }

    Trancate::Trancate(std::fstream& db_s,char name_table): Handler_c(db_s), table_name(name_table)
    {}
    std::string Trancate::handle_command()const
    {
        db_stream.seekg(0,std::ios::beg);
        Table tab;
        while (db_stream.read((char*)&tab,sizeof(Table)))
        {
            if(tab.table_name == this->table_name)
            {
                db_stream.seekg(0,std::ios::cur);
                auto current = db_stream.tellg();
                db_stream.seekg(0,std::ios::beg);
                auto offset = current - db_stream.tellg();
                db_stream.clear();
                db_stream.seekp(offset - sizeof(Table),std::ios::beg);
                tab.table_name = 'C';
                db_stream.write((char*)&tab,sizeof(Table));
                db_stream.clear();
                db_stream.seekg(offset,std::ios::beg);
            }
        }
        db_stream.clear();

        return "OK";
    }

    Intersection::Intersection(std::fstream& db_s): Handler_c(db_s)
    {}
    std::string Intersection::handle_command()const
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

        return makeResponse(result);
    }

    Symmetric_difference::Symmetric_difference(std::fstream& db_s): Handler_c(db_s)
    {}
    std::string Symmetric_difference::handle_command()const
    {
        auto all_data = readAll();
        std::map<size_t,std::pair<std::string,std::string>> result;
        std::set<char> tab_name;
        std::for_each(std::begin(all_data),std::end(all_data),[&tab_name](const Table& tab){
            if(tab.table_name == 'A') tab_name.insert('A');
            else if (tab.table_name == 'B') tab_name.insert('B');
        });
        if(tab_name.find('A')!=tab_name.end() && tab_name.find('B')!=tab_name.end())
        {
            std::for_each(all_data.begin(), all_data.end(), [&all_data,&result](const Table& tab){ 
            if(tab.table_name == 'A' || tab.table_name == 'B')
            {
                auto it = std::find_if(std::begin(all_data),std::end(all_data),[&tab](const Table& tab_){
                    if(tab_.id == tab.id && tab_.table_name!=tab.table_name && tab_.table_name!= 'C') return true;
                    else return false;
                });
                if(it== all_data.end())
                {
                    if(tab.table_name == 'A') result[tab.id] = std::make_pair(tab.name,"");
                    else result[tab.id] = std::make_pair("",tab.name);
                }
            }
            });
        }
        return makeResponse(result);
    }

    Defragmintation_db::Defragmintation_db(std::fstream& db_stream,std::string db_name): Handler_c(db_stream),db_name(db_name)
    {
    }
    std::string Defragmintation_db::handle_command()const
    {
        std::string copy = "copy";
        std::ofstream o_copy(copy,std::ios::ate | std::ios::binary);
        if(o_copy.is_open())
        {
            Table tab;
            db_stream.seekg(0,std::ios::beg);
            while (db_stream.read((char*)&tab,sizeof(Table)))
            {
                if(tab.table_name != 'C')
                {
                    o_copy.write((char*)&tab,sizeof(Table));
                }
            }
            db_stream.clear();
        }
        o_copy.close();
        db_stream.close();
        fs::remove(fs::path(db_name));
        fs::rename(fs::path(copy),fs::path(db_name));
        db_stream.open(db_name,std::ios::ios_base::in | std::ios::ios_base::out | std::ios::ios_base::binary);
        if(!db_stream.is_open()) return "Error open DB after defragmintation\n";
        return "OK";
    }


