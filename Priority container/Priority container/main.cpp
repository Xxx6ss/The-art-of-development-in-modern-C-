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
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class Type{
public:
    Type(T&& obj) : data_(move(obj)),
                    priority_(0)  {}
    
    void operator= (Type<T>&& type) {
        data_ = move(type.data_);
        priority_ = move(type.priority_);
//        return *this;
        
    }
    
    
    T data_;
    size_t priority_;
};


template <typename T>
class PriorityCollection {
public:
    using Id =  size_t;   /* тип, используемый для идентификаторов */


  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        conteiner[last_id] = pair<T, size_t>(move(object), 0);
        ++last_id;
        return last_id - 1;
    }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
      while (range_begin != range_end) {
          *ids_begin = Add(*range_begin);
      }
  }


  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        if (conteiner.count(id) == 1)
            return true;
        return false;
    }

  // Получить объект по идентификатору
    const T& Get(Id id) const {
        return conteiner[id];
    }


  // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        ++conteiner[id].second;
    }


  // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        auto el = find(conteiner.begin(), conteiner.end(),
                      [](std::map<Id, pair<T, size_t>> lhs, std::map<Id, pair<T, size_t>> rhs){
            return lhs.second.second == rhs.second.second;
        });
        return el.second;
    }

  // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto el = find(conteiner.begin(), conteiner.end(),
                      [](std::map<Id, pair<T, size_t>> lhs, std::map<Id, pair<T, size_t>> rhs){
            return lhs.second.second == rhs.second.second;
        });
        conteiner.erase(el);
        return el.second;
    }

private:
  // Приватные поля и методы
    std::map<Id, pair<T, size_t>> conteiner;
    Id last_id = 0;
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
