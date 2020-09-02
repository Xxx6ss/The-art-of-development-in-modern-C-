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

using namespace std;

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        if (liberated_.size()) {
            auto it = allocated_.insert(liberated_.front());
            liberated_.pop();
            return *(it.first);
        }
        else {
            T* obj = new T;
            auto it = allocated_.insert(move(obj));
            return *(it.first);
        }
    }
    T* TryAllocate() {
        if (liberated_.size()) {
            auto it = allocated_.insert(liberated_.front());
            liberated_.pop();
            return *(it.first);
        }
        else
            return nullptr;
    }

    void Deallocate(T* object) {
        if (allocated_.count(object) == 0)
            throw invalid_argument("Element not allocated");
        else {
            liberated_.push(object);
            allocated_.erase(object);
        }
    }

    ~ObjectPool() {
        for (auto it : allocated_) {
            delete it;
        }
        
        while(!liberated_.empty()) {
            delete (liberated_.front());
            liberated_.pop();
        }
    }

private:
    void Destroy_vector(std::vector<T*> &v)
    {
        while(!v.empty()) {
            delete (v.back());
            v.pop_back();
        }
    }
    
    set<T*> allocated_;
    queue<T*> liberated_;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

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
