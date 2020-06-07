//
//  main.cpp
//  Group strings
//
//  Created by Andrew Kireev on 06.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;


// Объявляем Groupдля произвольного типа String
// синонимом vector.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector.
template <typename String>
using Group = vector<String>;


// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char
template <typename String>
using Char = typename String::value_type;




template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {

    // Напишите реализацию функции,
    // использовав не более 1 копирования каждого символа
    vector<Group<String>> groups;
    for (auto item : strings) {
        if (groups.size() == 0) {
            groups.push_back(vector<string>{string(item)});
            continue;
        }
        int counter = 0;
        int iterations = 0;
        bool added = false;
        auto it = groups.begin();
        for (auto symbol = item.begin(); symbol != item.end(); ++symbol) {
            if (added == true)
                break;
            ++iterations;
            while (true) {
                if (it == groups.end()) {
                    groups.push_back(vector<string>{string(item)});
                    added = true;
                    break;
                }
                
                if ((*it)[0].find(*symbol) == (*it)[0].npos) {
                    counter = 0;
                    ++it;
                    symbol = item.begin();
                    continue;
                }
                else
                    ++counter;
                if (counter == item.size()) {
                    it->push_back(item);
                    added = true;
                    break;
                }
                break;
            }
        }
//        if (iterations == item.size() && added == false) {
//            groups.push_back(vector<string>{string(item)});
//        }
    }
    return groups;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups)); // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups)); // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
