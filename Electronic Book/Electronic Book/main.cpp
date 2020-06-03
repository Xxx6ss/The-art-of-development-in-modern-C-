//
//  main.cpp
//  Electronic Book
//
//  Created by Andrew Kireev on 01.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <iomanip>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <array>
#include "profile.h"

using namespace std;


class ReadingManager {
public:
  ReadingManager() {
      std::fill(pages.begin(), pages.begin() + MAX_USER_COUNT_, 0);
  }

  void Read(int user_id, int page_count) {
      if (pages[user_id] == 0)
          ++users_amount;
      else {
          users[pages[user_id]].erase(user_id);
      }
      users[page_count].insert(user_id);
      pages[user_id] = page_count;
  }

  double Cheer(int user_id) const {
      
      if (pages[user_id] == 0) {
          return 0;
          }
      auto it = users.find(pages[user_id]);
      if (users_amount == 1)
          return 1.0;
      if (it == users.begin())
          return 0.0;
      
      int count = 0;
      for(auto iter = users.begin(); iter != it; ++iter) {
          count += iter->second.size();
      }
      if (count == 0)
          return 0;
      return (count) * 1.0 / (users_amount - 1);
  }

private:
  static const int MAX_USER_COUNT_ = 100'000;

    map<int, set<int>> users;
    array<int, MAX_USER_COUNT_> pages;
    int users_amount = 0;

};


void time_test() {
    
    ReadingManager manager;
    
    {
        LOG_DURATION("READ");
        for (int i = 0; i < 10000; ++i) {
            manager.Read(i, i * 120);
        }
    }
    
    
    {
         LOG_DURATION("CHEER");
        for (int i = 0; i < 10000; ++i) {
            manager.Cheer(i);
        }
    }
    
    
}



int main() {

  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
    
//    time_test();

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;
      
    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}
