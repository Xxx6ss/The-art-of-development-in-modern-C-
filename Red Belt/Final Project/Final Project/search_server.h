//
//  search_server.hpp
//  Final Project
//
//  Created by Andrew Kireev on 19.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <future>
using namespace std;




template <typename T>
class Synchronized {
public:
    explicit Synchronized(T initial = T()) : value(move(initial)) {}

    struct Access {
        T& ref_to_value;
        lock_guard<mutex> guard;
    };

    Access GetAccess() {
        return { value, lock_guard(m) };
    }
private:
    mutex m;
    T value;
};

class InvertedIndex {
public:
    struct DocRating {
           size_t doc_id_;
           size_t rating_;
       };
    
    InvertedIndex() = default;
    explicit InvertedIndex(istream& document_input);
  void Add(istream& documents);
  vector<InvertedIndex::DocRating> Lookup( string& word) const;

  auto& GetDocument() const {
    return docs;
  }
    
    const size_t docs_size() const {
          return docs.size();
      }

private:
    
  map<string, vector<DocRating>> index;
  deque<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
//    explicit SearchServer(istream& document_input)
//      : index(InvertedIndex(document_input)){ }
//  void UpdateDocumentBaseAsync(istream& document_input);
    
    SearchServer(istream& document_input);
 void AddQueriesStream(istream& query_input, ostream& search_results_output);
//    void AddQueriesStreamAsync(istream& query_input, ostream& search_results_output,
//                               Synchronized<InvertedIndex>& ind);
    
    void UpdateDocumentBase(istream& document_input);

private:
  Synchronized<InvertedIndex> index;
    vector<future<void>> futures;
};
