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
#include <utility>
#include <array>
#include "profile.h"

using namespace std;

//class User {
//public:
//    User() = default;
//    User(int id, int readed) : id_(id), pages_readed_(readed) {}
//    int id_ = 0;
//    int pages_readed_ = 0;
//    bool operator< (const User& lhs, const User& rhs) const {
//     return lhs.pages_readed_ < rhs.pages_readed_;
//    }
//};




class ReadingManager {
public:
  ReadingManager(){
      std::fill(users.begin(), users.begin() + MAX_USER_COUNT_, 5555544);
     }

  void Read(int user_id, int page_count) {
    
//    User user(user_id, page_count);
      users[user_id] = page_count;
//      cout << users[user_id] << endl;
  }

  double Cheer(int user_id) const {
      if (users[user_id] == 5555544) {
          return 0.0;
    }
      
      int pages  = users[user_id];
      auto count = count_pages(pages);
      return count;
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  array<int, MAX_USER_COUNT_> users;
    
    
    float count_pages(int pages) const{
        int less = 0;
        int is_equal = 0;
        int not_null = 0;
        for(int pag : users) {
            if (pag < pages && pag != 5555544)
                ++less;
            if (pag == 5555544)
                ++is_equal;
            else
                ++not_null;
        }
        
        
        if (is_equal == MAX_USER_COUNT_ - 1) {
//            cout << "op" << endl;
            return 1;
            
        }
//        cout << "less " <<less << " pages " <<  pages<<endl;
        if (less == 0)
            return 0;
        return less * 1.0 / (not_null - 1);
    }


//  int GetUserCount() const {
//    return sorted_users_.size();
//  }
//
//  void SwapUsers(int lhs_position, int rhs_position) {
//    const int lhs_id = sorted_users_[lhs_position];
//    const int rhs_id = sorted_users_[rhs_position];
//    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
//    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
//  }
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
