#include "gtest/gtest.h"

#include "../include/skip_list.h"

#include  <vector>

using namespace skip_list;

class Skip_list_iterator : public ::testing::Test {
protected:

    virtual void SetUp()
    {      
        std::vector<std::pair<int, int>> insertValues =
        {
            { std::make_pair( 1 ,  10 ) },
            { std::make_pair( 2 ,  11 ) },
            { std::make_pair( 3 ,  12 ) },
            { std::make_pair( 4 ,  13 ) },
            { std::make_pair( 5 ,  14 ) },
            { std::make_pair( 6 ,  15 ) },
        };

        for (const auto& insertValue : insertValues) {
            obj.insert(insertValue);
        }
    }

    Skip_list<int, int> obj;
};

TEST_F(Skip_list_iterator, notEqual)
{
    auto it1 = obj.begin();
    auto it2 = obj.end();

    EXPECT_TRUE(it1 != it2);    
}

TEST_F(Skip_list_iterator, equal)
{
    auto it1 = obj.begin();
    auto it2 = obj.end();

    EXPECT_TRUE(it1 == it2);    
}