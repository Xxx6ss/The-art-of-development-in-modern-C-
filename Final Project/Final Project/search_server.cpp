//
//  search_server.cpp
//  Final Project
//
//  Created by Andrew Kireev on 19.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "search_server.hpp"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>



vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = move(new_index);
}



void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
    
    vector<pair<size_t, size_t>> v;
    vector<size_t> indexes;
    v.reserve(50010);
    v.resize(50010);
    
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWords(current_query);
      v.clear();
      indexes.clear();
      vector<pair<size_t, size_t>> search_results;
      for (int i = 0; i < 50'000; ++i) {
          search_results.push_back({i, 0});
      }
      
    map<size_t, size_t> docid_count;
    for (const auto& word : words) {
      for (const size_t docid : index.Lookup(word)) {
          search_results.at(docid).second++;
      }
    }

      
//      for (auto i : docid_count) {
//          search_results.push_back({i.first, i.second});
//      }
      
      
    sort(
      begin(search_results),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
  }
}






void InvertedIndex::Add(const string& document) {
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
  for (const auto& word : SplitIntoWords(document)) {
    index[word].push_back(docid);
  }
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
