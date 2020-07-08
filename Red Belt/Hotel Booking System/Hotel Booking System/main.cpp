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
#include <string>
#include <list>
#include "profile.h"

#include "test_runner.h"




class BookingSystem {
public:
    BookingSystem() = default;
    
    void book(int64_t time, const std::string& hotel_name, int64_t client_id, int room_count) {
    
        hotel_clients_[hotel_name][client_id] += room_count;
        hotel_rooms_[hotel_name] += room_count;
        clients_.push_back(Client(hotel_name, client_id, time, room_count));
        
        remove_old(time);
        
    }
    
    uint64_t clients(const std::string& hotel_name) {
        return hotel_clients_[hotel_name].size();
    }
    
    uint64_t rooms(const std::string& hotel_name) {
        return hotel_rooms_[hotel_name];
    }
    
    
    
private:
    
    std::map<std::string, std::map<uint64_t, uint64_t>> hotel_clients_;             //clients in hotels
    std::map<std::string, uint64_t> hotel_rooms_;                          //time for every client
    static const int DAY_TIME_ = 86400;
    
    class Client {
    public:
        
        Client(std::string hotel_name, uint64_t client_id, uint64_t time, int rooms) :
        hotel_name_(hotel_name), client_id_(client_id), time_(time), room_count_(rooms) {}
        
        std::string hotel_name_;
        uint64_t client_id_;
        int64_t time_;
        int room_count_;
       };
    
    std::list<Client> clients_;
    
    void remove_old(int64_t time) {
        while (clients_.front().time_ + 86400 <= time && !clients_.empty()) {
            Client elem = clients_.front();
            hotel_clients_[elem.hotel_name_][elem.client_id_] -= elem.room_count_;
            if (hotel_clients_[elem.hotel_name_][elem.client_id_] == 0) {
                  hotel_clients_[elem.hotel_name_].erase(elem.client_id_);
                }
            hotel_rooms_[elem.hotel_name_] -= elem.room_count_;
            clients_.pop_front();
        }
    }
};







int main(int argc, const char * argv[]) {
    

     ios::sync_with_stdio(false);
     cin.tie(nullptr);
    
    int query_count;
    std::cin >> query_count;
    BookingSystem sys;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        std::string query_type;
        std::cin >> query_type;

      if (query_type == "BOOK") {
          unsigned int client_id, room_count;
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
          std::cout <<sys.clients(hotel_name) << endl;
        

      } else if (query_type == "ROOMS") {
          std::string hotel_name;
          std::cin >> hotel_name;
          std::cout << sys.rooms(hotel_name) << endl;
      }
    }
}
