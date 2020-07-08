//
//  main.cpp
//  secondary_index
//
//  Created by Andrew Kireev on 08.07.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
    
    struct Iterators {
        std::multimap<int, Record*>::iterator time_iterator;
        std::multimap<int, Record*>::iterator karma_iterator;
        std::multimap<string, Record*>::iterator user_iterator;
    } iterators;
    
};



class Database {
public:
    bool Put(const Record& record) {
        auto [it, result] = record_by_id_.try_emplace(record.id, record);
        
        if (result) {
            auto addr = std::addressof(it->second);
            it->second.iterators = {
                range_by_time_.emplace(record.timestamp, addr),
                range_by_karma_.emplace(record.karma, addr),
                range_by_user_.emplace(record.user, addr)
            };
            
            return true;
        }
        return false;
    }
    const Record* GetById(const string& id) const {
        if (record_by_id_.count(id) == 0)
            return nullptr;
        return std::addressof(record_by_id_.at(id));
    }
    bool Erase(const string& id) {
        auto it = record_by_id_.find(id);
        
        if (it == record_by_id_.end())
            return false;
        
        range_by_time_.erase(it->second.iterators.time_iterator);
        range_by_karma_.erase(it->second.iterators.karma_iterator);
        range_by_user_.erase(it->second.iterators.user_iterator);
        record_by_id_.erase(it);
        return true;
    }
    
    
    template <typename Key, typename Callback>
    void Range(const multimap<Key, Record*>& con, const Key& low, const Key& high,
               Callback callback) const {
        if (low > high)
            return;
        
        auto l = con.lower_bound(low);
        auto h = con.upper_bound(high);
        for (auto it = l; it != h && callback(*it->second); ++it);
    }
    

  template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        Range(range_by_time_, low, high, callback);
    }

  template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        Range(range_by_karma_, low, high, callback);
    }

  template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        Range(range_by_user_, user, user, callback);
    }
private:
    std::unordered_map<string, Record> record_by_id_;
    std::multimap<int, Record*> range_by_time_;
    std::multimap<int, Record*> range_by_karma_;
    std::multimap<string, Record*> range_by_user_;
    
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
