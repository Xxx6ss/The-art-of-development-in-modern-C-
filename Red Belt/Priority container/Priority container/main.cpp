//
//  main.cpp
//  Priority container
//
//  Created by Andrew Kireev on 09.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <unordered_map>

using namespace std;




template <typename T>
class PriorityCollection {
public:
    using Id =  size_t;   /* тип, используемый для идентификаторов */


  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        data[last_id] = move(object);
        priority[last_id] = 0;
        to_find_priority_[0].push_back(last_id);
        ++last_id;
        return last_id - 1;
    }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
      while(range_begin != range_end) {
          *ids_begin = Add(*range_begin);
          ++ids_begin;
          ++range_begin;
      }
  }


  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        if (data.count(id) > 0)
            return true;
        return false;
    }

  // Получить объект по идентификатору
    const T& Get(Id id) const {
        return data.at(id);
    }


  // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        Id present_prior = priority[id];
        to_find_priority_[present_prior].erase(find(to_find_priority_[present_prior].begin(),
                                                    to_find_priority_[present_prior].end(), id)
                                               );
        
        to_find_priority_[present_prior + 1].insert(upper_bound(begin(to_find_priority_[present_prior + 1]),
                                                                end(to_find_priority_[present_prior + 1]), id), id
                                                    );
        
        if (to_find_priority_[present_prior].empty()) {
            to_find_priority_.erase(present_prior);
        }
        ++priority[id];
    }


  // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        auto el = prev(to_find_priority_.end(), 1);
        return  pair<const T&, int>{data.at(el->second.back()), el->first};
    }

  // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
       auto el = prev(to_find_priority_.end(), 1);

       pair<T, int> res = { move(data[el->second.back()]), el->first };
        
       data.erase(el->second.back());
       priority.erase(el->second.back());
       to_find_priority_[el->first].pop_back();
        
       if (to_find_priority_[el->first].empty()) {
           to_find_priority_.erase(el->first);
       }
       return res;
    }

private:
  // Приватные поля и методы
    map<Id, T> data;
    map<Id, size_t> priority;
    map<int, vector<Id>> to_find_priority_;
    size_t id_max_priority_ = 0;
    int last_id = 0;
    
};


class StringNonCopyable : public string {
public:
  using string::string; // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};


void TestNoCopy() {
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");
    const auto yellow_id = strings.Add("yellow");
    const auto red_id = strings.Add("red");
    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i) {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    {
        const auto item = strings.PopMax();
        
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}






