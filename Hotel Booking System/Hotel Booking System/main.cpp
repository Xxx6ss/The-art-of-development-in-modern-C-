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
    
    void book(int client_id, const std::string& hotel_name, int room_count, int64_t& time) {
        if (book_time_[hotel_name].size() < 10)
            book_time_[hotel_name].resize(10);

        hotel_clients_[hotel_name].push_back(client_id);
        book_time_[hotel_name][client_id] = time;
        rooms_time_[hotel_name].push_back(std::pair(room_count, time));
        for (auto it : book_time_) {
            for (auto item : it.second) {
                if ((time - item >= DAY_TIME_ || item < -DAY_TIME_) && item != 0) {
                    auto iter = std::find(book_time_[hotel_name].begin(), book_time_[hotel_name].end(), item);
    //                book_time_[hotel_name].erase(iter);
                    *iter = 0;
                }
            }
        }
        for (auto it = rooms_time_.begin(); it != rooms_time_.end(); ++it) {
            for (auto item = it->second.begin(); item != it->second.end(); ++item) {
                if ((time - item->second >=  DAY_TIME_ || item->second < -DAY_TIME_) && item->second != 0) {
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
            if (item != 0)
                ++count;
        }
        std::cout << count << std::endl;
    }
    
    void rooms(const std::string& hotel_name) {
        int count = 0;
        for (auto item : rooms_time_[hotel_name]) {
            count += item.first;
        }
        std::cout << count << std::endl;
    }
    
    
    
private:
    std::map<std::string, std::vector<int>> hotel_clients_;             //clients in hotels
    std::map<std::string, std::vector<int64_t>> book_time_;                          //time for every client
    std::map<std::string, std::vector<std::pair<int, int64_t>>> rooms_time_;                          //when room was booked in every hotel
    static const int DAY_TIME_ = 86400;
    
    
    
    
};


int main(int argc, const char * argv[]) {
    
    
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
          sys.book(client_id, hotel_name, room_count, time);

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
