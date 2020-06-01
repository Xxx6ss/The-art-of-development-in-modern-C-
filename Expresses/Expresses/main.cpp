//
//  main.cpp
//  Expresses
//
//  Created by Andrew Kireev on 01.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include "profile.h"
#include <unordered_map>

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
      reachable_lists_[start].insert(finish);
      reachable_lists_[finish].insert(start);
  }

 int FindNearestFinish(int start, int finish) const {
     int result = abs(start - finish);
     if (reachable_lists_.count(start) < 1) {
         return result;
     }
     if (!reachable_lists_.at(start).empty()) {
       result = min(result, abs(finish - *min_element(
                   begin(reachable_lists_.at(start)), end(reachable_lists_.at(start)),
                   [finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
               )));
     }
     return result;
   }
private:
    map<int, set<int>> reachable_lists_;
};

int test() {
  RouteManager routes;
    set<int> s;


    {

        LOG_DURATION("ADD");
        for (int query_id = 0; query_id < 1000000; ++query_id) {
            routes.AddRoute(query_id, query_id * 5);
        }
    }



    {
        LOG_DURATION("GO");
      for (int query_id = 0; query_id < 10000000; ++query_id) {
          routes.FindNearestFinish(query_id, query_id * 5);
      }
    }

  return 0;
}


int main() {
    test();
  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}
