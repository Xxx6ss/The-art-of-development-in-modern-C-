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
            if (!most_pop_name_.count(item.name))
                most_pop_name_[item.name] = 1;
            else
                most_pop_name_[item.name]++;
        }
    }
    
    string find_most_popular_name() const {
        string name = "";
        int count = 0;
        
        for (auto item : most_pop_name_) {
            if (count < item.second) {
                count = item.second;
                name = item.first;
            }
        }
        return name;
    }
    long find_age(int age) const {
        auto adult_begin = lower_bound(
          begin(srt_by_age_), end(srt_by_age_), age, [](const Person& lhs, long age) {
            return lhs.age < age;
          }
        );
        return std::distance(adult_begin, end(srt_by_age_));
    }
public:
    vector<Person> srt_by_age_;
    vector<Person> srt_by_income_;
    unordered_map<string, int> most_pop_name_;
};

Data ReadPeople(istream& input) {
  long count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
      if (p.income < 0)                     /// Mb need to be removed
          p.income = 0;
    p.is_male = (gender == 'M');
  }

    sort(begin(result), end(result), [](const Person& lhs, const Person& rhs) {
      return lhs.age < rhs.age;
    });
  return Data(result);
}

int main() {
    const Data people = ReadPeople(cin);

//  sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
//    return lhs.age < rhs.age;
//  });

  for (string command; cin >> command; ) {
    if (command == "AGE") {
      long adult_age;
      cin >> adult_age;

      

        cout << "There are " << people.find_age(adult_age)
        << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY") {
      long count;
      cin >> count;

      auto head = Head(people, count);

      partial_sort(
        head.begin(), head.end(), end(people), [](const Person& lhs, const Person& rhs) {
          return lhs.income > rhs.income;
        }
      );

    
      long total_income = accumulate(
        head.begin(), head.end(), 0, [](int& cur, Person& p) {
          return cur += p.income;
        }
      );
      cout << "Top-" << count << " people have total income " << total_income << '\n';
    total_income = 0;
    } else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;

      IteratorRange range{
        begin(people),
        partition(begin(people), end(people), [gender](Person& p) {
          return p.is_male == (gender == 'M');
        })
      };
      if (range.begin() == range.end()) {
        cout << "No people of gender " << gender << '\n';
      } else {
        sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs) {
          return lhs.name < rhs.name;
        });
        const string* most_popular_name = &range.begin()->name;
        long count = 1;
        for (auto i = range.begin(); i != range.end(); ) {
          auto same_name_end = find_if_not(i, range.end(), [i](const Person& p) {
            return p.name == i->name;
          });
          long cur_name_count = std::distance(i, same_name_end);
          if (cur_name_count > count) {
            count = cur_name_count;
            most_popular_name = &i->name;
          }
          i = same_name_end;
        }
        cout << "Most popular name among people of gender " << gender << " is "
             << *most_popular_name << '\n';
      }
    }
  }
}
