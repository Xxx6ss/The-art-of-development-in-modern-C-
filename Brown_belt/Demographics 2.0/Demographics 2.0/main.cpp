//
//  main.cpp
//  Demographics 2.0
//
//  Created by Andrew Kireev on 18.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <unordered_map>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  long age, income;
  bool is_male;
};

class Data {
public:
    Data(vector<Person> persons){
        sort(begin(persons), end(persons), [](const Person& lhs, const Person& rhs) {
          return lhs.age < rhs.age;
        });
        srt_by_age_ = persons;
        
        sort(begin(persons), end(persons), [](const Person& lhs, const Person& rhs) {
          return lhs.income < rhs.income;
        });
        srt_by_income_ = persons;
        
        for (auto item : persons) {
            if (item.is_male) {
                if (!most_pop_name_M.count(item.name))
                    most_pop_name_M[item.name] = 1;
                else
                    most_pop_name_M[item.name]++;
            }
            else {
                if (!most_pop_name_W.count(item.name))
                    most_pop_name_W[item.name] = 1;
                else
                    most_pop_name_W[item.name]++;
            }
        }
    }
    
    string find_most_popular_name_W() const {
        string name = "";
        int count = 0;
        
        for (auto item : most_pop_name_W) {
            if (count < item.second) {
                count = item.second;
                name = item.first;
            }
        }
        return check_strings(name, most_pop_name_W, count);
    }
    
    string find_most_popular_name_M() const {
        string name = "";
        int count = 0;
        
        for (auto item : most_pop_name_M) {
            if (count < item.second) {
                count = item.second;
                name = item.first;
            }
        }
        return check_strings(name, most_pop_name_M, count);
    }
    long find_age(int age) const {
        auto adult_begin = lower_bound(
          begin(srt_by_age_), end(srt_by_age_), age, [](const Person& lhs, long age) {
            return lhs.age < age;
          }
        );
        return std::distance(adult_begin, end(srt_by_age_));
    }
    
    long find_wealth(int num) const{
        long amount = 0;
        auto it = srt_by_income_.end();
        it = it - num;
        for (;it != srt_by_income_.end(); ++it)
            amount += it->income;
        return amount;
    }
    
    bool is_no_people(char gender) const {
        if (gender == 'W') {
            if (most_pop_name_W.begin() == most_pop_name_W.end())
                return true;
        } else if (gender == 'M') {
            if (most_pop_name_M.begin() == most_pop_name_M.end())
                return true;
        }
        return false;
    }
private:
    vector<Person> srt_by_age_;
    vector<Person> srt_by_income_;
    unordered_map<string, int> most_pop_name_M;
    unordered_map<string, int> most_pop_name_W;
    
    string check_strings(string name, unordered_map<string, int> names, int count) const {
        for (auto it : names) {
            if (it.second == count && it.first < name)
                name = it.first;
        }
        return name;
    }
};

Data ReadPeople(istream& input) {
  long count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = (gender == 'M');
  }

    sort(begin(result), end(result), [](const Person& lhs, const Person& rhs) {
      return lhs.age < rhs.age;
    });
  return Data(result);
}

int main() {
    const Data people = ReadPeople(cin);

  for (string command; cin >> command; ) {
    if (command == "AGE") {
      long adult_age;
      cin >> adult_age;

      cout << "There are " << people.find_age(adult_age)
        << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY") {
      long count;
      cin >> count;

        cout << "Top-" << count << " people have total income " << people.find_wealth(count) << '\n';
    } else if (command == "POPULAR_NAME") {
        char gender;
        cin >> gender;
        if (people.is_no_people(gender)) {
            cout << "No people of gender " << gender << '\n';
            continue;
        } else {
            if (gender == 'W') {
                cout << "Most popular name among people of gender " << gender << " is "
                << people.find_most_popular_name_W() << '\n';
            } else if (gender == 'M') {
                cout << "Most popular name among people of gender " << gender << " is "
                << people.find_most_popular_name_M() << '\n';
            }
        }
    }
  }
}
