//
//  main.cpp
//  Demographic indicators
//
//  Created by Andrew Kireev on 09.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>



enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};



// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  std::vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

void PrintStats(std::vector<Person> persons) {
    
    std::cout << "Median age = " +
    std::to_string(ComputeMedianAge(persons.begin(), persons.end())) << std::endl;
    
    auto it = std::partition(persons.begin(), persons.end(),   
                           [](const Person& el) {
        return el.gender == Gender::FEMALE;
    });

    std::cout << "Median age for females = " +
    std::to_string(ComputeMedianAge(persons.begin(), it)) << std::endl;
    
    it = std::partition(persons.begin(), persons.end(),     =
                           [](const Person& el) {
        return el.gender == Gender::MALE;
    });

    std::cout << "Median age for males = " +
    std::to_string(ComputeMedianAge(persons.begin(), it)) << std::endl;
    
    it = std::partition(persons.begin(), persons.end(),
                           [](const Person& el) {
        return el.gender == Gender::FEMALE and
        el.is_employed == true;
    });

    std::cout << "Median age for employed females = " +
    std::to_string(ComputeMedianAge(persons.begin(), it)) << std::endl;
    
    it = std::partition(persons.begin(), persons.end(),
                           [](const Person& el) {
        return el.gender == Gender::FEMALE and
        el.is_employed == false;
    });

    std::cout << "Median age for unemployed females = " +
    std::to_string(ComputeMedianAge(persons.begin(), it)) << std::endl;
    
    it = std::partition(persons.begin(), persons.end(),
                           [](const Person& el) {
        return el.gender == Gender::MALE and
        el.is_employed == true;
    });

    std::cout << "Median age for employed males = " +
    std::to_string(ComputeMedianAge(persons.begin(), it)) << std::endl;
    
    it = std::partition(persons.begin(), persons.end(),
                           [](const Person& el) {
        return el.gender == Gender::MALE and
        el.is_employed == false;
    });

    std::cout << "Median age for unemployed males = " +
    std::to_string(ComputeMedianAge(persons.begin(), it)) << std::endl;
}

int main() {
  std::vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}
