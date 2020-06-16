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
using namespace std;

struct Stats {
  map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (const auto& item : other.word_frequences) {
            word_frequences[item.first] = item.second;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    size_t pos = 0;
    for (auto word : key_words) {
        while (line.find(word, pos) != line.npos) {
            pos = line.find(word, pos);
            if (pos != line.npos && (pos  + word.size() == line.size() ||
                                     line.at(pos + word.size()) == ' ' )) {
                ++result.word_frequences[word];
            }
            pos += word.size();
        }
        pos = 0;
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream& input
) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

//Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
//  // Р РµР°Р»РёР·СѓР№С‚Рµ СЌС‚Сѓ С„СѓРЅРєС†РёСЋ
//}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWordsSingleThread(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
//  TestRunner tr;
//  RUN_TEST(tr, TestBasic);
    
    
    Stats st;
    set<string> words = {"privet", "poka", "opa"};
    string str = "ehal opa jepa ooooooora poka, poka poka privet";
    st += ExploreLine(words, str);
    return 0;
}












