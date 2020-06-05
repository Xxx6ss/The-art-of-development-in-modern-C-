//
//  main.cpp
//  Hotel Booking System
//
//  Created by Andrew Kireev on 03.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <iostream>
#include <cstdint>
#include <map>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <utility>
#include <array>
#include <queue>
#include "profile.h"

#include "test_runner.h"

using namespace std;

class Rooms {
public:
    Rooms (int amount, int64_t time) : amount_(amount), time_(time) {}
    int amount_;
    int64_t time_;
    
    friend bool operator== (const Rooms& lhs, const Rooms& rhs) {
        return lhs.time_ == rhs.time_ && lhs.amount_ == rhs.amount_;
    }
};


class BookingSystem {
public:
    BookingSystem() {
    }
    
    void book(int64_t time, const std::string& hotel_name, int client_id, int room_count) {
        if (book_time_[hotel_name].size() == 0) {
             book_time_[hotel_name].reserve(10);
            book_time_[hotel_name].resize(10, std::pair(-1, -1));
        } else if (book_time_[hotel_name].size() <= client_id) {
            book_time_[hotel_name].reserve((client_id + 1) * 2);
            book_time_[hotel_name].resize(client_id * 2, std::pair(-1, -1));
        }
            

//        if (time == 0)
//            null_time[client_id] = 0;
        
        hotel_clients_[hotel_name].push_back(client_id);
        book_time_[hotel_name][client_id] = std::pair(client_id, time);
        rooms_time_[hotel_name].push_back(std::pair(room_count, time));
//        for (auto it : book_time_) {
//            for (auto item : it.second) {
//                if (((item == 0) ? (item  + 1 + DAY_TIME_<= time) : (item + DAY_TIME_<= time)) && item != 0) {
//                    auto iter = std::find(book_time_[hotel_name].begin(), book_time_[hotel_name].end(), item);
//    //                book_time_[hotel_name].erase(iter);
//                    *iter = 0;
//                }
//            }
//        }
        for (auto it = book_time_.begin(); it != book_time_.end(); ++it) {
                   for (auto item = it->second.begin(); item != it->second.end(); ++item) {
//                       auto new_time = (item->second == 0) ? 1 : item->second;
                       if (((item->second + DAY_TIME_<= time)) && (item->first != -1)) {
           //                auto iter = std::find(rooms_time_[hotel_name].begin(), rooms_time_[hotel_name].end(),
           //                                      [](std::pair<int, int64_t> lhs, std::pair<int, int64_t> rhs) {
           //                    return lhs.first == rhs.first && lhs.second == rhs.second;
           //                });
                           item->first = -1;
                           item->second = -1;
                       }
                   }
               }
        
        for (auto it = rooms_time_.begin(); it != rooms_time_.end(); ++it) {
            for (auto item = it->second.begin(); item != it->second.end(); ++item) {
                auto new_time = (item->second == 0) ? 1 : item->second;
                if (((new_time + DAY_TIME_<= time)) && (item->first != 0)) {
    //                auto iter = std::find(rooms_time_[hotel_name].begin(), rooms_time_[hotel_name].end(),
    //                                      [](std::pair<int, int64_t> lhs, std::pair<int, int64_t> rhs) {
    //                    return lhs.first == rhs.first && lhs.second == rhs.second;
    //                });
                    item->first = 0;
                    item->second = 0;
                }
            }
        }
        
    }
    
    void clients(const std::string& hotel_name) {
        int count = 0;
        for (auto item : book_time_[hotel_name]) {
            if (item.first != -1)
                ++count;
        }
        std::cout << count << std::endl;
        return count;
    }
    
    void rooms(const std::string& hotel_name) {
        int count = 0;
        for (auto item : rooms_time_[hotel_name]) {
            count += item.first;
        }
        std::cout << count << std::endl;
        return count;
    }
    
    
    
private:
    std::map<std::string, std::vector<int>> hotel_clients_;             //clients in hotels
    std::map<std::string, std::vector<std::pair<int, int64_t>>> book_time_;                          //time for every client
    std::map<std::string, std::vector<std::pair<int, int64_t>>> rooms_time_;                          //when room was booked in every hotel
    static const int DAY_TIME_ = 86400;
    std::map<int, int> null_time;
    
    
    
    
};



struct Event {
  int64_t time;
  std::string hotel_name;
  unsigned int client_id;
  int room_count;
};


struct TestQuery {
  std::string query;
  int64_t time;
  std::string hotel_name;
  unsigned int client_id;
  int room_count;
};

void testBookingManager() {
  vector<TestQuery> queries =
  {
    {"CLIENTS", 0, "Marriott", 0, 0},
    {"ROOMS", 0, "Marriott", 0, 0},
    {"book", 10, "FourSeasons", 1, 2},
    {"book", 10, "Marriott", 1, 1},
    {"book", 86409, "FourSeasons", 2, 1},
    {"CLIENTS", 0, "FourSeasons", 0, 0},
    {"ROOMS", 0, "FourSeasons", 0, 0},
    {"CLIENTS", 0, "Marriott", 0, 0},
    {"book", 86410, "Marriott", 2, 10},
    {"ROOMS", 0, "FourSeasons", 0, 0},
    {"ROOMS", 0, "Marriott", 0, 0}
  };

  BookingSystem manager;
  vector<uint64_t> result;
  for (const auto& q : queries) {
    if (q.query == "book") {
      manager.book(q.time, q.hotel_name, q.client_id, q.room_count);
    } else if (q.query == "CLIENTS") {
      result.push_back(manager.clients(q.hotel_name));
    } else {
      result.push_back(manager.rooms(q.hotel_name));
    }
  }

  vector<uint64_t> expected = {0, 0, 2, 3, 1, 1, 10};
  ASSERT_EQUAL(result, expected);
}

void testRooms() {
  {
    vector<Event> queries;
    BookingSystem manager;

    for (int i = 0; i < 10; ++i) {
      queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 2});
      queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), 2});
    }

    for (const auto& q : queries) {
      manager.book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.rooms("Marriott"), 20);
    ASSERT_EQUAL(manager.rooms("FourSeasons"), 20);
  }
  {
    vector<Event> queries;
    BookingSystem manager;

    for (int i = 0; i < 86500; ++i) {
      queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 1});
      queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), 1});
    }

    for (const auto& q : queries) {
      manager.book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.rooms("Marriott"), 86400);
    ASSERT_EQUAL(manager.rooms("FourSeasons"), 86400);
  }
}

void testClients() {
  {
    vector<Event> queries;
    BookingSystem manager;

    for (int i = 0; i < 10; ++i) {
      queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 2});
      queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), 2});
    }

    for (const auto& q : queries) {
      manager.book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.clients("Marriott"), 1);
    ASSERT_EQUAL(manager.clients("FourSeasons"), 10);
  }
  {
    vector<Event> queries;
    BookingSystem manager;

    for (int i = 0; i < 86500; ++i) {
       queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 1});
       queries.push_back({static_cast<int64_t>(i), "FourSeasons", 1, 1});
    }

    for (const auto& q : queries) {
      manager.book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.clients("Marriott"), 1);
    ASSERT_EQUAL(manager.clients("FourSeasons"), 1);
  }

}

void stressTest() {
  vector<Event> queries;
  for (int i = 0, j = 0; i < 100000; ++i) {
    if (j % 100 == 0) {
      ++j;
    }
    queries.push_back({static_cast<int64_t>(i), "Marriott", static_cast<unsigned int>(i), j});
    queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), j});
  }

  BookingSystem manager;
  {
    LOG_DURATION("book");
    for (const auto& q : queries) {
      manager.book(q.time, q.hotel_name, q.client_id, q.room_count);
    }
  }

  vector<Event> queries1;
  for (int i = 0; i < 50000; ++i) {
    queries1.push_back({0, "Marriott", 0, 0});
    queries1.push_back({0, "FourSeasons", 0, 0});
  }

  {
    LOG_DURATION("clients");
    for (const auto& q : queries1) {
      manager.clients(q.hotel_name);
    }
  }

  vector<TestQuery> queries2;
  for (int i = 0; i < 50000; ++i) {
    queries2.push_back({"ROOMS", 0, "Marriott", 0, 0});
    queries2.push_back({"ROOMS", 0, "FourSeasons", 0, 0});
  }

  {
    LOG_DURATION("rooms");
    for (const auto& q : queries2) {
      manager.rooms(q.hotel_name);
    }
  }
}


int main(int argc, const char * argv[]) {
    
     TestRunner tr;
     RUN_TEST(tr, testBookingManager);
     RUN_TEST(tr, testRooms);
     RUN_TEST(tr, testClients);
     stressTest();

     ios::sync_with_stdio(false);
     cin.tie(nullptr);
    
    int query_count;
    std::cin >> query_count;
    BookingSystem sys;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;

      if (query_type == "BOOK") {
          int client_id, room_count;
          int64_t time;
          std::string hotel_name;
          std::cin >> time;
          std::cin >> hotel_name;
          std::cin >> client_id;
          std::cin >> room_count;
          sys.book( time, hotel_name, client_id, room_count);

      } else if (query_type == "CLIENTS") {
          std::string hotel_name;
          std::cin >> hotel_name;
          sys.clients(hotel_name);
        

      } else if (query_type == "ROOMS") {
          std::string hotel_name;
          std::cin >> hotel_name;
          sys.rooms(hotel_name);
      }
    }
}
