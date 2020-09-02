//
//  animals.h
//  Zoo
//
//  Created by Andrew Kireev on 02.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef animals_h
#define animals_h

#include <string>

class Animal {
public:
  virtual std::string Voice() const {
    return "Not implemented yet";
  }
  virtual ~Animal() {
  }
};


class Tiger: public Animal {
  std::string Voice() const override {
    return "Rrrr";
  }
};


class Wolf: public Animal {
  std::string Voice() const override {
    return "Wooo";
  }
};


class Fox: public Animal {
  std::string Voice() const override {
    return "Tyaf";
  }
};

#endif /* animals_h */
