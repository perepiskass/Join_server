#pragma once
#include <fstream>


/**
 * @brief Класс, для хранения состояния подключения к файлу БД и создания запросов в нее.
 */
class HandlerDB
{
    public:
        HandlerDB(const std::string& name);
        std::string setCommand(const std::string& command);
        ~HandlerDB();

    private:
        void checkCommand(const std::string& command);
        const std::string db_name;
        std::fstream db_stream;
        std::string response;
};
