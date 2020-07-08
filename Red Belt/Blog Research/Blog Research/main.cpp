//
//  main.cpp
//  Blog Research
//
//  Created by Andrew Kireev on 16.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <functional>
#include <future>
#include <string_view>
#include <utility>
using namespace std;

#define TREADS = 8;

struct Stats {
  map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (const auto& item : other.word_frequences) {
            word_frequences[item.first] += item.second;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    string_view str = line;
    string word;
    while(true) {
        size_t space = str.find_first_of(' ');
        if (space == str.npos)
            word = string(str.begin(), str.end());
        else
            word = string(str.begin(), space);
        if (key_words.count(word))
            ++result.word_frequences[word];
        if (space == str.npos){
            break;
            
        } else {
            str.remove_prefix(space + 1);
            
        }
    }
    return result;
}

//Stats ExploreKeyWordsSingleThread(
//  const set<string>& key_words, istream& input
//) {
//  Stats result;
//  for (string line; getline(input, line); ) {
//    result += ExploreLine(key_words, line);
//  }
//  return result;
//}


using vec_iter = vector<string>::iterator;

Stats ExploreKeyWordsSingleThread(const set<string>& key_words,
                                  vec_iter begin, vec_iter end) {
  Stats result;
  while(begin != end) {
    result += ExploreLine(key_words, *begin);
      ++begin;
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    
    vector<future<Stats>> futures;
    string str;
    vector<string> strings;
    
    while(getline(input, str)) {
        strings.push_back(move(str));
    }
    
    vector<string>::iterator iter = strings.begin();
    size_t step = strings.size() / 10 + strings.size() % 10;
    
    while(iter  + step < strings.end()) {
        futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), iter, iter + step));
        advance(iter, step);
    }
    futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), iter, strings.end()));
//    cout << futures.size() << endl;
    Stats result;
    for (auto& item : futures) {
        result += item.get();
    }
    
    return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);

    return 0;
}












