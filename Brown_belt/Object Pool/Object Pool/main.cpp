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
            allocated_.push_back(liberated_.front());
            liberated_.erase(liberated_.begin());
            return (allocated_.back());
        }
        else {
            T* obj = new T;
            allocated_.push_back(obj);
            return (allocated_.back());
        }
    }
    T* TryAllocate() {
        if (liberated_.size()) {
            allocated_.push_back(liberated_.front());
            liberated_.erase(liberated_.begin());
            return (allocated_.back());
        }
        else
            return nullptr;
    }

    void Deallocate(T* object) {
        if (std::count(allocated_.begin(), allocated_.end(), object) == 0)
            throw invalid_argument("Element not allocated");
        else {
            liberated_.push_back(*std::find(allocated_.begin(),
                                                 allocated_.end(), object));
            allocated_.erase(std::find(allocated_.begin(),
                                        allocated_.end(), object));
        }
    }

    ~ObjectPool() {
        Destroy_vector(allocated_);
        Destroy_vector(liberated_);
    }

private:
    void Destroy_vector(std::vector<T*> &v)
    {
        while(!v.empty()) {
            delete (v.back());
            v.pop_back();
        }
    }
    
    vector<T*> allocated_;
    vector<T*> liberated_;
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
