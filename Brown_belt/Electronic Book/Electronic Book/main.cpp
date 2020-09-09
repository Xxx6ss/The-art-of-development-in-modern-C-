//
//  main.cpp
//  Electronic Book
//
//  Created by Andrew Kireev on 09.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//


#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, -1),
        users_on_page_(1000 + 1, 0) {
        }

  void Read(size_t user_id, size_t page_count) {
      int i  = user_page_counts_[user_id] + 1;
      for (; i < page_count + 1; ++i) {
          ++users_on_page_[i];
      }
      
      user_page_counts_[user_id] = page_count;
  }

  double Cheer(size_t user_id) const {
      int pages_readed = user_page_counts_[user_id];
      if (pages_readed == -1)
          return 0;
      int users_readed_page = users_on_page_[pages_readed];
      int users_amount = Get_users_amount();
      if (users_amount == 1)
          return 1;
      
      return (users_amount - (users_readed_page)) * 1.0 / (users_amount - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT
    static const int MAX_USER_COUNT_ = 100'000;
    vector<size_t> user_page_counts_;
    vector<size_t> users_on_page_;

    int Get_users_amount() const {
        return users_on_page_[0];
    }
};


int main() {
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
