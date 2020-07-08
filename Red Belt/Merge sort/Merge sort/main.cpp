//
//  main.cpp
//  Merge sort
//
//  Created by Andrew Kireev on 08.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if ((range_end - range_begin) < 2)
        return;
    
    vector<typename RandomIt::value_type> vec(make_move_iterator(range_begin),
                                              make_move_iterator(range_end));
    
    size_t length = vec.size() / 3;
    
    MergeSort(vec.begin(), vec.begin() + length);
    MergeSort(vec.begin() + length, vec.begin() + 2 * length);
    MergeSort(vec.begin() + 2 * length, vec.end());
    
    
    vector<typename RandomIt::value_type> temper_vec;
    merge(make_move_iterator(vec.begin()), make_move_iterator(vec.begin() + length),
          make_move_iterator(vec.begin() + length), make_move_iterator(vec.begin() + 2 * length),
          back_inserter(temper_vec)
    );
    
    
    merge(make_move_iterator(temper_vec.begin()), make_move_iterator(temper_vec.end()),
          make_move_iterator(vec.begin() + 2 * length), make_move_iterator(vec.end()),
          range_begin
    );
    
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
