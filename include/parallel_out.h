#pragma once
#include <mutex>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>

#include "logger.h"

using Time = std::chrono::seconds;
using Time_point = std::chrono::_V2::steady_clock::time_point;
using Bulk = std::pair<std::vector<std::string>,Time>;

/**
 * @brief Класс для вывода информации(в файл, в консоль) в параллельной среде.
 */
class Writer:public std::stringstream
{
    private:
        static inline std::mutex cout_mutex;
    public:
    static void console(const std::vector<std::string>& bulks, int id)
    {
        std::lock_guard<std::mutex> lcout {cout_mutex};
        std::cout << "bulk " << id << ": ";
        for(auto str = bulks.begin(); str!=bulks.end(); ++str)
        {
            Logger::getInstance().set_commandCount(id);
            if(str==bulks.begin()) std::cout << *str;
            else std::cout << ", " << *str;
        }
        std::cout << std::endl;
    }

    static void file(const Bulk& bulks, int id, Time_point start)
    {
        std::ofstream out;
        auto timeUNIX = bulks.second.count();
        auto end(std::chrono::steady_clock::now());
        using nanoseconds = std::chrono::duration<double,std::ratio<1,1'000'000'000>>;
        auto diff = nanoseconds(end - start).count();
        std::string path = "bulk"+ std::to_string(timeUNIX) + '.' + std::to_string(int(diff)) + ".log";
        out.open(path);
        if (out.is_open(),std::ios::app)
        {
            out << "bulk " << id << ": ";
            for(auto str = bulks.first.begin(); str!=bulks.first.end(); ++str)
            {
                Logger::getInstance().set_commandCount(id);
                if(str==bulks.first.begin()) out << *str;
                else out << ", " << *str;
            }
        }
        out.close();
    }
    ~Writer()
    {
        std::lock_guard<std::mutex> l {cout_mutex};
        std::cout << rdbuf();
        std::cout.flush();
    }
};