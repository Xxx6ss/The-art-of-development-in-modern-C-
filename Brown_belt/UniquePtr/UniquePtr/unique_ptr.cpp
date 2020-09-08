//
//  main.cpp
//  UniquePtr
//
//  Created by Andrew Kireev on 08.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"

#include <cstddef> // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* ptr_;
public:
    UniquePtr() : ptr_(new T) {}
    
    UniquePtr(T * ptr) : ptr_(ptr){
}
    UniquePtr(const UniquePtr& other) {
        ptr_ = std::move(other.Get());
    }
    UniquePtr(UniquePtr&& other) {
        this = std::move(other);
    }
    
    UniquePtr& operator = (const UniquePtr& other) {
        ptr_ = std::move(other.Get());
    }
    UniquePtr& operator = (nullptr_t other) {
        ptr_ = nullptr;
    }
    UniquePtr& operator = (UniquePtr&& other) {
        ptr_ = std::move(other.Get());
    }
    ~UniquePtr() {
        delete ptr_;
    }

    T& operator * () const {
        return *ptr_;
    }

    T * operator -> () const{
        return ptr_;
    }

    T * Release() {
        T* new_ptr = ptr_;
        ptr_ = nullptr;
        return new_ptr;
    }

    void Reset(T * ptr) {
        delete ptr_;
        ptr_ = std::move(ptr);
    }

    void Swap(UniquePtr& other) {
        T* new_ptr = ptr_;
        ptr_ = other.Get();
        other.Reset(new_ptr);
    }

    T * Get() const {
        return ptr_;
    }
};


struct Item {
  static int counter;
  int value;
  Item(int v = 0): value(v) {
    ++counter;
  }
  Item(const Item& other): value(other.value) {
    ++counter;
  }
  ~Item() {
    --counter;
  }
};

int Item::counter = 0;


void TestLifetime() {
  Item::counter = 0;
  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    ptr.Reset(new Item);
    ASSERT_EQUAL(Item::counter, 1);
  }
  ASSERT_EQUAL(Item::counter, 0);

  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    auto rawPtr = ptr.Release();
    ASSERT_EQUAL(Item::counter, 1);

    delete rawPtr;
    ASSERT_EQUAL(Item::counter, 0);
  }
  ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
  UniquePtr<Item> ptr(new Item(42));
  ASSERT_EQUAL(ptr.Get()->value, 42);
  ASSERT_EQUAL((*ptr).value, 42);
  ASSERT_EQUAL(ptr->value, 42);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
}

