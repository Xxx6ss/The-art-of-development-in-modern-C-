//
//  parse.hpp
//  Final Project
//
//  Created by Andrew Kireev on 19.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp
#pragma once

#include <stdio.h>


#include "iterator_range.h"

#include <string_view>
#include <sstream>
#include <vector>
using namespace std;

template <typename Container>
string Join(char c, const Container& cont) {
  ostringstream os;
  for (const auto& item : Head(cont, cont.size() - 1)) {
    os << item << c;
  }
  os << *rbegin(cont);
  return os.str();
}

string_view Strip(string_view s);
vector<string_view> SplitBy(string_view s, char sep);
#endif /* parse_hpp */
