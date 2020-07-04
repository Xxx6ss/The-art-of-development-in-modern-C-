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
using namespace std;

class InvertedIndex {
public:
    struct DocRating {
           size_t doc_id_;
           size_t rating_;
       };
    
    InvertedIndex() = default;
  void Add(istream& documents);
  vector<InvertedIndex::DocRating> Lookup(string& word) const;

  const deque<string>& GetDocument() const {
    return docs;
  }

private:
    
  map<string, vector<DocRating>> index;
  deque<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
