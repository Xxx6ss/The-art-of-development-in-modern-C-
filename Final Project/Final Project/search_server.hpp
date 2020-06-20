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
#include <string>
using namespace std;

class InvertedIndex {
public:
  void Add(const string& document);
  list<size_t> Lookup(const string& word) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

private:
  map<string, list<size_t>> index;
  vector<string> docs;
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
