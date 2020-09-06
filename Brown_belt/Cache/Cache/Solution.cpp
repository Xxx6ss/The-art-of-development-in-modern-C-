//
//  Solution.cpp
//  Cache
//
//  Created by Andrew Kireev on 06.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "Common.h"
#include <map>

class Book : public IBook {
public:
    Book(std::string name, std::string content) :
                            name_(name), content_(content) {}
    
    const std::string& GetName() const {
        return name_;
    }
    
    const std::string& GetContent() const {
        return content_;
    }
    
private:
    std::string name_;
    std::string content_;
};


class LruCache : public ICache {
public:
  LruCache(
      std::shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) {
    // реализуйте метод
  }

  BookPtr GetBook(const std::string& book_name) override {
    
  }
private:
    std::multimap<int, std::string> rang_to_book_name_;
    std::unordered_map<std::string, BookPtr> name_to_book_;
};


std::unique_ptr<ICache> MakeCache(
    std::shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  // реализуйте функцию
}
