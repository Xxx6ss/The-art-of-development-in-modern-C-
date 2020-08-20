//
//  lazy_value.cpp
//  Lazy Value
//
//  Created by Andrew Kireev on 20.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <iostream>

#include "test_runner.h"

#include <functional>
#include <optional>
#include <string>
#include <memory>
using namespace std;

template <typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init) : init_(init) {}

    bool HasValue() const {
        if (!value_)
            return false;
        return true;
    }
    const T& Get() const {
        if (!value_)
            value_ = init_();
        return *value_;
    }

private:
    std::function<T()> init_;
    mutable std::optional<T> value_;
};

void UseExample() {
  const string big_string = "Giant amounts of memory";

  LazyValue<string> lazy_string([&big_string] { return big_string; });

  ASSERT(!lazy_string.HasValue());
  ASSERT_EQUAL(lazy_string.Get(), big_string);
  ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
  bool called = false;

  {
    LazyValue<int> lazy_int([&called] {
      called = true;
      return 0;
    });
  }
  ASSERT(!called);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, UseExample);
  RUN_TEST(tr, TestInitializerIsntCalled);
  return 0;
}
