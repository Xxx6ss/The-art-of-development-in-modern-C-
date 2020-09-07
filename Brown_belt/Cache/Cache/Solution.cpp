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
#include <atomic>
#include <algorithm>

class LruCache : public ICache {
private:
        struct Book_Rang {
            BookPtr book_;
            int rang_ = 0;
        };
    
    using set_of_books = std::unordered_map<std::string, Book_Rang>;
public:
  LruCache(
      std::shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) : unpacker_(move(books_unpacker)), settings_(settings) { }

  BookPtr GetBook(const std::string& book_name) override {
      std::lock_guard guard(mutex_);
      
      auto it = name_to_book_.find(book_name);
      
      if (it == name_to_book_.end()) {
          Book_Rang new_book;
          new_book.book_ = unpacker_->UnpackBook(book_name);
          

          
          if (new_book.book_->GetContent().size() < settings_.max_memory) {
              size_t book_size = new_book.book_->GetContent().size();
              
              
              new_book.rang_ = ++max_rang_;
              auto iter = name_to_book_.insert({book_name, new_book});
              current_memory_ += book_size;
              
              while(current_memory_ > settings_.max_memory && !name_to_book_.empty()) {
                  if (name_to_book_.size() == 0)
                      break;
                  auto to_remove = std::min_element(name_to_book_.begin(), name_to_book_.end(),
                            [](const set_of_books::value_type& lhs, const set_of_books::value_type& rhs) {
                      return lhs.second.rang_ < rhs.second.rang_;
                  });
                  current_memory_ -= to_remove->second.book_->GetContent().size();
                  name_to_book_.erase(to_remove);
              }
              
              if (book_size > settings_.max_memory) {
                  return move(new_book.book_);
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
    int current_memory_ = 0;
};


std::unique_ptr<ICache> MakeCache(
    std::shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
    return std::make_unique<LruCache>(books_unpacker, settings);
}
