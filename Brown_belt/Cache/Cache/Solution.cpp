//
//  Solution.cpp
//  Cache
//
//  Created by Andrew Kireev on 06.09.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "Common.h"
#include <map>
#include <unordered_map>
#include <mutex>

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
private:
        struct Book_Rang {
    //        Book_Rang() = default;
    //
    //        Book_Rang(std::unique_ptr<IBook> book, int max_rang) :
    //        book_(std::make_shared<Book>(*book)), rang_(max_rang) {
    //        }
            
            BookPtr book_;
            int rang_ = 0;
        };
    
    using set_of_books = std::unordered_map<std::string, Book_Rang>;
public:
  LruCache(
      std::shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) : unpacker_(move(books_unpacker)), settings_(settings){
    // реализуйте метод
  }

  BookPtr GetBook(const std::string& book_name) override {
      std::lock_guard guard(mutex_);
      
      auto it = name_to_book_.find(book_name);
      
      if (it == name_to_book_.end()) {
          Book_Rang new_book;
          new_book.book_ = unpacker_->UnpackBook(book_name);
          if (max_rang_ < settings_.max_memory) {
              new_book.rang_ = ++max_rang_;
              int size_of_book = new_book.book_->GetContent().size();
              current_memory_ += size_of_book;
              auto iter = name_to_book_.insert({book_name, std::move(new_book)});
              while(current_memory_ > settings_.max_memory) {
                  auto to_remove = std::min_element(name_to_book_.begin(), name_to_book_.end(),
                            [](const set_of_books::value_type& lhs, const set_of_books::value_type& rhs) {
                      return lhs.second.rang_ < rhs.second.rang_;
                  });
                  
                  current_memory_ -= to_remove->second.book_->GetContent().size();
                  name_to_book_.erase(to_remove);
              }
              return iter.first->second.book_;
          }
          else
              return new_book.book_;
          
      } else {
          it->second.rang_++;
          return it->second.book_;
      }
  }
private:
    
    std::shared_ptr<IBooksUnpacker> unpacker_;
    std::unordered_map<std::string, Book_Rang> name_to_book_;
    const Settings& settings_;
    std::mutex mutex_;
    int max_rang_ = 0;
    std::atomic<int> current_memory_ = 0;
};


std::unique_ptr<ICache> MakeCache(
    std::shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  // реализуйте функцию
    return std::make_unique<LruCache>(books_unpacker, settings);
}
