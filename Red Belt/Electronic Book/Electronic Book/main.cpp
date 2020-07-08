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
#include "test_runner.h"
#include "profile.h"

using namespace std;


class ReadingManager {
public:
  ReadingManager() {
      std::fill(pages.begin(), pages.begin() + MAX_USER_COUNT_, 0);
      std::fill(users.begin(), users.begin() + MAX_USER_COUNT_, 0);
      std::fill(rating.begin(), rating.begin() + MAX_USER_COUNT_, 0);
  }

  void Read(int user_id, int page_count) {
//      if (page_count > MAX_PAGES_)
//          return ;
      if (users[user_id] == 0)
          ++users_amount;
      else {
          --pages[users[user_id]];
      }
      users[user_id] = page_count;
      ++pages[page_count];
      if (page_count < page_read)
          page_read = page_count;
  }

  double Cheer(int user_id)  {

      if (users[user_id] == 0) {
          return 0;
          }
      if (users_amount == 1)
          return 1.0;
      if (users[user_id] <= page_read)
          return 0.0;

       rating[users[user_id]] = count_rating(users[user_id]);
      
      return rating[users[user_id]];
  }

private:
  static const int MAX_PAGES_ = 1000;
  static const int MAX_USER_COUNT_ = 100'001;

    array<int, MAX_USER_COUNT_> users;
    array<int, MAX_USER_COUNT_> pages;
    array<float, MAX_USER_COUNT_> rating;
    int users_amount = 0;
    int page_read = 99999999;
    

    float count_rating(const int page) const {
        int count = 0;;
        for(int i = 0; i < page; ++i) {
            count += pages[i];
        }
        auto num = (count) * 1.0 / (users_amount - 1);
//        cout << num << endl;
        return num;
    }

};

int main() {
  // tests

  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

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
