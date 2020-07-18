#include "gtest/gtest.h"

#include "../include/skip_list.h"

#include <vector>

using namespace skip_list;

class Skip_list_iterator : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        std::vector<std::pair<int, int>> insertValues = {
            {std::make_pair(1, 10)}, {std::make_pair(2, 11)},
            {std::make_pair(3, 12)}, {std::make_pair(4, 13)},
            {std::make_pair(5, 14)}, {std::make_pair(6, 15)},
        };

        for (const auto& insertValue : insertValues) {
            obj.insert(insertValue);
        }
    }

    Skip_list<int, int> obj;
};

TEST_F(Skip_list_iterator, operator_asignment)
{
    auto it1 = obj.begin();
    auto it2 = it1;

    EXPECT_EQ(it1 , it2);
    EXPECT_EQ(it1->first, it2->first);
    EXPECT_EQ(it1->second, it2->second);
}

TEST_F(Skip_list_iterator, operator_equal)
{
    auto it1 = obj.begin();
    auto it2 = obj.end();

    EXPECT_TRUE(it1 == it2);
}

TEST_F(Skip_list_iterator, operator_notEqual)
{
    auto it1 = obj.begin();
    auto it2 = obj.end();

    EXPECT_TRUE(it1 != it2);
}

TEST_F(Skip_list_iterator, operator_bigger)
{
    auto it1 = obj.begin();
    auto it2 = ++it1;
    auto it3 = ++it2;

    EXPECT_FALSE(it1 > it1);
    EXPECT_FALSE(it1 > it2);
    EXPECT_FALSE(it1 > it3);
    EXPECT_TRUE(it2 > it1);
    EXPECT_FALSE(it2 > it2);
    EXPECT_FALSE(it2 > it3);
    EXPECT_TRUE(it3 > it1);
    EXPECT_TRUE(it3 > it2);
    EXPECT_FALSE(it3 > it3);

    auto itEnd = obj.end();

    EXPECT_TRUE(itEnd > it1);
    EXPECT_TRUE(itEnd > it2);
    EXPECT_TRUE(itEnd > it3);
}

TEST_F(Skip_list_iterator, operator_plusplus)
{
    auto it1 = obj.begin();
    auto it2 = ++it1;

    EXPECT_EQ(it1->first, 1);
    EXPECT_EQ(it2->first, 2);
}

TEST_F(Skip_list_iterator, operator_plus_equal)
{
    auto it1 = obj.begin();
    it1 += 1;

    EXPECT_EQ(it1->first, 1);
}

TEST_F(Skip_list_iterator, operator_plus)
{
    auto it1 = obj.begin();
    auto it2 = it1 + 1;

    EXPECT_EQ(it1->first, 1);
    EXPECT_EQ(it2->first, 2);
}

TEST_F(Skip_list_iterator, operator_asteriks)
{
    auto it1 = obj.begin();

    EXPECT_EQ(*it1, 10);
}

TEST_F(Skip_list_iterator, operator_arrow)
{
    auto it1 = obj.begin();

    EXPECT_EQ(it1->second, 10);
}

TEST(Skip_list, copy_constructor)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    Skip_list<int, int> copy{ obj };
    EXPECT_EQ(obj[1], 10);
    EXPECT_EQ(copy[1], 10);
}

TEST(Skip_list, copy_asignment)
{
    Skip_list<int, int> obj1;
    obj1.insert(std::make_pair(1, 10));

    Skip_list<int, int> obj2;
    obj2.insert(std::make_pair(2, 20));

    EXPECT_EQ(obj1[1], 10);
    EXPECT_EQ(obj2[2], 20);

    obj2 = obj1;

    EXPECT_EQ(obj1[1], 10);
    EXPECT_EQ(obj2[1], 10);
    EXPECT_EQ(obj2.find(2), obj2.end());
}

TEST(Skip_list, move_constructor)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    Skip_list<int, int> copy{ std::move(obj) };
    EXPECT_EQ(obj[1], 10);
    EXPECT_EQ(copy[1], 10);
    EXPECT_TRUE(obj.empty());
}

TEST(Skip_list, move_asignment)
{
    Skip_list<int, int> obj1;
    obj1.insert(std::make_pair(1, 10));

    Skip_list<int, int> obj2;
    obj2.insert(std::make_pair(2, 20));

    EXPECT_EQ(obj1[1], 10);
    EXPECT_EQ(obj2[2], 20);

    obj2 = std::move(obj1);

    EXPECT_TRUE(obj1.empty());
    EXPECT_EQ(obj2.find(2), obj2.end());
}

TEST(Skip_list, iterator_begin)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    auto it = obj.begin();
    EXPECT_EQ(*it, 1);
}

TEST(Skip_list, iterator_end)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    auto it = obj.end();
    EXPECT_EQ(it, nullptr);
}

TEST(Skip_list, const_iterator_begin)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    const auto it = obj.begin();
    EXPECT_EQ(*it, 1);
}

TEST(Skip_list, const_iterator_end)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    const auto it = obj.end();
    EXPECT_EQ(it, nullptr);
}

TEST(Skip_list, const_iterator_cbegin)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    auto it = obj.cbegin();
    EXPECT_EQ(*it, 1);
}

TEST(Skip_list, const_iterator_cend)
{
    Skip_list<int, int> obj;
    obj.insert(std::make_pair(1, 10));

    auto it = obj.cend();
    EXPECT_EQ(it, nullptr);
}

TEST(Skip_list, empty)
{
    Skip_list<int, int> obj;

    EXPECT_TRUE(obj.empty());

    obj.insert({std::make_pair(1, 10)});

    EXPECT_FALSE(obj.empty());
}

TEST(Skip_list, size)
{
    Skip_list<int, int> obj;

    EXPECT_EQ(obj.size(), 0);

    obj.insert({std::make_pair(1, 10)});

    EXPECT_EQ(obj.size(), 1);

    obj.insert({std::make_pair(2, 20)});

    EXPECT_EQ(obj.size(), 2);
}


TEST(Skip_list, maxSize)
{
    Skip_list<int, int> obj;

    EXPECT_EQ(obj.max_size(), (std::size_t{} - 1));
}

TEST(Skip_list, operator_const_access)
{
    Skip_list<int, int> obj;
    obj.insert({std::make_pair(1, 10)});

    const int key = 1;
    EXPECT_EQ(obj[1], 10);
}

TEST(Skip_list, operator_access)
{
    Skip_list<int, int> obj;
    obj.insert({std::make_pair(1, 10)});

    const int key = 1;
    EXPECT_EQ(obj[1], 10);
}

TEST(Skip_list, insert)
{
    Skip_list<int, int> obj;
    auto it_and_result = obj.insert({std::make_pair(1, 10)});

    EXPECT_EQ(it_and_result.first->first, 1);
    EXPECT_EQ(it_and_result.first->first, 10);
    EXPECT_TRUE(it_and_result.second);
}

TEST(Skip_list, insert_fails)
{
    Skip_list<int, int> obj;
    obj.insert({std::make_pair(1, 10)});

    auto it_and_result = obj.insert({std::make_pair(2, 10)});

    EXPECT_EQ(it_and_result, nullptr);
    EXPECT_FALSE(it_and_result.second);
}

TEST(Skip_list, clear)
{
    Skip_list<int, int> obj;
    obj.insert({std::make_pair(1, 10)});

    EXPECT_EQ(obj.size(), 1);

    obj.clear();

    EXPECT_EQ(obj.size(), 0);
}

TEST(Skip_list, find_result_end)
{
    Skip_list<int, int> obj;

    Skip_list<int, int>::iterator it = obj.find(1);

    EXPECT_EQ(it, obj.end());
}

TEST(Skip_list, const_find_result_end)
{
    const Skip_list<int, int> obj;

    Skip_list<int, int>::const_iterator it = obj.find(1);

    EXPECT_EQ(it, obj.end());
}

TEST(Skip_list, find_result_find_begin)
{
    Skip_list<int, int> obj;

    obj.insert({std::make_pair(1, 10)});
    obj.insert({std::make_pair(2, 20)});
    obj.insert({std::make_pair(3, 30)});
    obj.insert({std::make_pair(4, 40)});
    obj.insert({std::make_pair(5, 50)});

    Skip_list<int, int>::iterator it = obj.find(1);

    auto key_value = *it;

    EXPECT_EQ(key_value.first, 1);
    EXPECT_EQ(key_value.second, 10);
}

TEST(Skip_list, const_find_result_find_begin)
{
    Skip_list<int, int> obj;

    obj.insert({std::make_pair(1, 10)});
    obj.insert({std::make_pair(2, 20)});
    obj.insert({std::make_pair(3, 30)});
    obj.insert({std::make_pair(4, 40)});
    obj.insert({std::make_pair(5, 50)});

    Skip_list<int, int>::const_iterator it = obj.find(1);

    auto key_value = *it;

    EXPECT_EQ(key_value.first, 1);
    EXPECT_EQ(key_value.second, 10);
}

TEST(Skip_list, find_result_find_middle)
{
    Skip_list<int, int> obj;

    obj.insert({std::make_pair(1, 10)});
    obj.insert({std::make_pair(2, 20)});
    obj.insert({std::make_pair(3, 30)});
    obj.insert({std::make_pair(4, 40)});
    obj.insert({std::make_pair(5, 50)});

    Skip_list<int, int>::iterator it = obj.find(3);

    auto key_value = *it;

    EXPECT_EQ(key_value.first, 3);
    EXPECT_EQ(key_value.second, 30);
}

TEST(Skip_list, const_find_result_find_middle)
{
    Skip_list<int, int> obj;

    obj.insert({std::make_pair(1, 10)});
    obj.insert({std::make_pair(2, 20)});
    obj.insert({std::make_pair(3, 30)});
    obj.insert({std::make_pair(4, 40)});
    obj.insert({std::make_pair(5, 50)});

    Skip_list<int, int>::const_iterator it = obj.find(3);

    auto key_value = *it;

    EXPECT_EQ(key_value.first, 3);
    EXPECT_EQ(key_value.second, 30);
}


TEST(Skip_list, find_result_find_last)
{
    Skip_list<int, int> obj;

    obj.insert({std::make_pair(1, 10)});
    obj.insert({std::make_pair(2, 20)});
    obj.insert({std::make_pair(3, 30)});
    obj.insert({std::make_pair(4, 40)});
    obj.insert({std::make_pair(5, 50)});

    Skip_list<int, int>::iterator it = obj.find(5);

    auto key_value = *it;

    EXPECT_EQ(key_value.first, 5);
    EXPECT_EQ(key_value.second, 50);
}



TEST(Skip_list, count)
{
    Skip_list<int, int> obj;

    EXPECT_EQ(obj.count(1), 0);

    obj.insert({std::make_pair(1, 10)});

    EXPECT_EQ(obj.count(1), 1);
}

TEST(Skip_list, top_level)
{
    Skip_list<int, int> obj;

    EXPECT_EQ(obj.top_level(), 0);

    obj.insert({std::make_pair(1, 10)});

    EXPECT_EQ(obj.top_level(), 1);
}


TEST(Skip_list, insert_and_erase)
{
    Skip_list<int, int> obj;
    std::vector<int> keys{1, 6, 2, 7, 3, 8, 4, 9, 5};

    for (const auto& key : keys) {
        auto position = obj.insert(std::make_pair(key, key + 10));

        EXPECT_EQ((*position.first).first, key);
        EXPECT_TRUE(position.second);
    }

    std::sort(keys.begin(), keys.end());

    for (const auto& key : keys) {
        EXPECT_EQ(obj.erase(key), 1);
    }

    constexpr auto keyNotInList = 100;
    EXPECT_EQ(obj.erase(keyNotInList), 0);

    EXPECT_TRUE(obj.empty());
}

TEST(Skip_list, insert_same_key_twice)
{
    Skip_list<int, int> obj;

    obj.insert(std::make_pair(1, 5));

    ASSERT_EQ(obj.size(), 1);
    EXPECT_EQ(obj[1], 5);

    obj.insert(std::make_pair(1, 10));

    ASSERT_EQ(obj.size(), 1);
    EXPECT_EQ(obj[1], 10);
}

TEST(Skip_list, iterator_find)
{
    Skip_list<int, int> obj;
    std::vector<int> keys{1, 6, 2, 7, 3, 8, 4, 9, 5};

    for (const auto& key : keys) {
        auto value = key + 10;
        obj.insert(std::make_pair(key, value));
    }
    std::sort(keys.begin(), keys.end());

    for (const auto& key : keys) {
        const int search_value = key + 10;

        auto it = obj.find(key);

        ASSERT_TRUE(it != obj.end());
        EXPECT_EQ(it->second, search_value);
    }

    const int invalid_key = keys.back() + 1;

    auto it = obj.find(invalid_key);

    EXPECT_TRUE(it == obj.end());
}

TEST(Skip_list, operator_access)
{
    Skip_list<int, int> obj;

    obj.insert(std::make_pair(1, 10));
    obj.insert(std::make_pair(2, 20));
    obj.insert(std::make_pair(3, 30));

    EXPECT_EQ(obj[1], 10);
    EXPECT_EQ(obj[2], 20);
    EXPECT_EQ(obj[3], 30);
}

TEST(Skip_list, operator_access_const)
{
    Skip_list<int, int> obj;

    obj.insert(std::make_pair(1, 10));
    obj.insert(std::make_pair(2, 20));
    obj.insert(std::make_pair(3, 30));

    const int key = 2;

    EXPECT_EQ(obj[key], 20);
}