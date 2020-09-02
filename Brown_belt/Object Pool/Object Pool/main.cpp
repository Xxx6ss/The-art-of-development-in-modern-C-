//
//  main.cpp
//  Object Pool
//
//  Created by Andrew Kireev on 01.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <memory>

using namespace std;

template <class T>
class ObjectPool {
public:
    shared_ptr<T> Allocate() {
        if (liberated_.size()) {
            shared_ptr<T> it = *(allocated_.insert(liberated_.front())).first;
            liberated_.pop();
            return it;
        }
        else {
            shared_ptr<T> it = *(allocated_.insert(make_shared<T>(§))).first;
            return it;
        }
    }
    shared_ptr<T> TryAllocate() {
        if (liberated_.size()) {
            shared_ptr<T> it = *(allocated_.insert(liberated_.front())).first;
            liberated_.pop();
            return it;
        }
        else
            return nullptr;
    }

    void Deallocate(shared_ptr<T> object) {
        if (allocated_.count(object) == 0)
            throw invalid_argument("Element not allocated");
        else {
            liberated_.push(object);
            allocated_.erase(object);
        }
    }


private:
    void Destroy_vector(std::vector<shared_ptr<T>> &v)
    {
        while(!v.empty()) {
            delete (v.back());
            v.pop_back();
        }
    }
    
    set<shared_ptr<T>> allocated_;
    queue<shared_ptr<T>> liberated_;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  shared_ptr<string> p1 = pool.Allocate();
  shared_ptr<string> p2 = pool.Allocate();
  shared_ptr<string> p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
