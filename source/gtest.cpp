#include <gtest/gtest.h>
#include "version_lib.h"
#include <db_handler.h>



// Тест кейсы на проверку версий
TEST(version_test_case, version_test_patch)
{
    ASSERT_GE(version_patch(), 1);
}
TEST(version_test_case, version_test_minor)
{
    EXPECT_EQ(version_minor(), 1);
}
TEST(version_test_case, version_test_major)
{
    EXPECT_EQ(version_major(), 1);
}

namespace fs = boost::filesystem;
// Teст кейсы на проверку базы данных
TEST(db_handler_test_case, create_db_test)
{
    for(size_t i = 0;i < 100;++i)
    {
        std::unique_ptr<HandlerDB> db_handle {new HandlerDB("test"+std::to_string(i))};
        auto exist = fs::exists(fs::path("test"+std::to_string(i)));
        EXPECT_TRUE(exist);
        fs::remove(fs::path("test"+std::to_string(i)));
    }
}

TEST(db_handler_test_case, insert_db_test)
{
    std::ofstream t("test",std::ios::ios_base::ate);
    t.close();
    std::fstream test ("test",std::ios::in | std::ios::out | std::ios::binary);
    if(test.is_open())
    {
        for(size_t i = 0;i < 100;++i)
        {
            char name[10];
            strcpy(name,"name");
            std::unique_ptr<Handler_c> insert{new Insert(test,'A',i,name)};
            auto result = insert->handle_command();
            EXPECT_EQ(result,"OK");
        }
    }
    else std::cout << "ERROR OPEN FILE\n";
    test.close();
    fs::remove(fs::path("test"));
}

TEST(db_handler_test_case, trancate_db_test)
{
    std::ofstream t("test",std::ios::ios_base::ate);
    t.close();
    std::fstream test ("test",std::ios::in | std::ios::out | std::ios::binary);
    if(test.is_open())
    {
        for(size_t i = 0;i < 100;++i)
        {
            char name[10];
            strcpy(name,"name");
            std::unique_ptr<Handler_c> insert{new Insert(test,'A',i,name)};
            auto result = insert->handle_command();
            std::unique_ptr<Handler_c> insert_dublicate{new Insert(test,'A',i,name)};
            result = insert_dublicate->handle_command();
            EXPECT_EQ(result,"ERR duplicate " + std::to_string(i));

            std::unique_ptr<Handler_c> trancate{new Trancate(test,'A')};
            result = trancate->handle_command();
            EXPECT_EQ(result,"OK");
            std::unique_ptr<Handler_c> try_insert{new Insert(test,'A',i,name)};
            result = try_insert->handle_command();
            EXPECT_EQ(result,"OK");
        }
    }
    else std::cout << "ERROR OPEN FILE\n";
    test.close();
    fs::remove(fs::path("test"));
}

TEST(db_handler_test_case, intersection_db_test)
{
    std::ofstream t("test",std::ios::ios_base::ate);
    t.close();
    std::fstream test ("test",std::ios::in | std::ios::out | std::ios::binary);
    std::string response;
    char name[10];
    strcpy(name,"name");

    if(test.is_open())
    {
        for(size_t i = 0;i < 100;++i)
        {
            std::unique_ptr<Handler_c> insertA{new Insert(test,'A',i,name)};
            insertA->handle_command();
            std::unique_ptr<Handler_c> insertB{new Insert(test,'B',i,name)};
            insertB->handle_command();
            std::unique_ptr<Handler_c> intersection{new Intersection(test)};
            auto result = intersection->handle_command();
            response+=std::to_string(i)+",name,name\n";
            EXPECT_EQ(result,response+"OK");
        }
    }
    else std::cout << "ERROR OPEN FILE\n";
    test.close();
    fs::remove(fs::path("test"));
}

TEST(db_handler_test_case, symmetric_difference_db_test)
{
    std::ofstream t("test",std::ios::ios_base::ate);
    t.close();
    std::fstream test ("test",std::ios::in | std::ios::out | std::ios::binary);
    std::string response;
    char name[10];
    strcpy(name,"name");

    if(test.is_open())
    {
        for(size_t i = 0;i < 50;++i)
        {
            std::unique_ptr<Handler_c> insertA{new Insert(test,'A',i,name)};
            insertA->handle_command();
            response+=std::to_string(i)+",name,"+"\n";
        }
        for(size_t i = 50;i < 100;++i)
        {
            std::unique_ptr<Handler_c> insertB{new Insert(test,'B',i,name)};
            insertB->handle_command();
            response+=std::to_string(i)+",,name\n";
        }
        std::unique_ptr<Handler_c> symmetric_difference{new Symmetric_difference(test)};
        auto result = symmetric_difference->handle_command();
        EXPECT_EQ(result,response+"OK");
    }
    else std::cout << "ERROR OPEN FILE\n";
    test.close();
    fs::remove(fs::path("test"));
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}