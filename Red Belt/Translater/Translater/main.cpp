//
//  main.cpp
//  Translater
//
//  Created by Andrew Kireev on 31.05.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "test_runner.h"
#include <string>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <deque>
#include <algorithm>

using namespace std;

class Translator {
public:
    Translator(){
    }
    void Add(string_view source, string_view target) {
        if (dictionar_forward.count(source) == 0) {
            words.push_back(string(source));
            words.push_back(string(target));
            dictionar_forward[*(words.end() - 2)] = target;
            dictionar_back[*(words.end() - 1)] = source;
        } else if (dictionar_back.count(target) == 0){
            words.push_back(string(target));
            dictionar_back[*(words.end() - 1)] = source;
            dictionar_forward[dictionar_back[*(words.end() - 1)]] = target;
        }
    }
    string_view TranslateForward(string_view source) const {
        if (dictionar_forward.count(source) != 0) {
            string_view s = dictionar_forward.at(source);
            return  s;
        }
        return string();
    }
    string_view TranslateBackward(string_view target) const {
        if (dictionar_back.count(target) != 0) {
            string_view s = dictionar_back.at(target);
            return s;
        }
        else
            return string();
    }

private:
    deque<string> words;
    unordered_map<string_view, string_view> dictionar_forward;
    unordered_map<string_view, string_view> dictionar_back;
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));
    translator.Add(string("privet"), string("poka"));


    auto s = translator.TranslateForward("okno");
    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");

    translator.Add(string("okno"), string("opapapa"));
    ASSERT_EQUAL(translator.TranslateForward("okno"), "opapapa");
    ASSERT_EQUAL(translator.TranslateBackward("opapapa"), "okno");
    ASSERT_EQUAL(translator.TranslateForward("privet"), "poka");
    ASSERT_EQUAL(translator.TranslateBackward("poka"), "privet");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
