//
//  search_server.cpp
//  Final Project
//
//  Created by Andrew Kireev on 19.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <numeric>






vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}



void UpdateDocumentBaseAsync (istream& document_input, Synchronized<InvertedIndex>& index) {
    InvertedIndex new_index;
    new_index.Add(document_input);
    swap(index.GetAccess().ref_to_value, new_index);
}


void SearchServer::UpdateDocumentBase(istream& document_input) {
    futures.push_back(async(UpdateDocumentBaseAsync, ref(document_input), ref(index)));
}






void AddQueriesStreamAsync(
  istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& ind
) {
    auto un_sync_index = ind.GetAccess().ref_to_value;
    auto documents = un_sync_index.GetDocument();
    vector<size_t> v(documents.size());
    vector<size_t> docid_count(documents.size());
  for (string current_query; getline(query_input, current_query); ) {
    auto words = SplitIntoWords(current_query);

    docid_count.assign(docid_count.size(), 0);
      
    for (string& word : words) {
        auto con = un_sync_index.Lookup(word);
        for (const auto& [docid, rating] : con) {
          docid_count[docid] += rating;
      }
    }
      
      iota(v.begin(), v.end(), 0);
      
    partial_sort(
      begin(v),
         Head(v, 5).end(),
         end(v),
      [&docid_count](int64_t lhs, int64_t rhs) {
          return make_pair(docid_count[lhs], -lhs) > make_pair(docid_count[rhs], -rhs);
      }
      );

    search_results_output << current_query << ':';
    for (size_t docid : Head(v, 5)) {
        
        size_t hitcount = docid_count[docid];
        
        if (hitcount == 0) {
            break;
        }
        
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
        
    }
      
    search_results_output << endl;
  }
}

void SearchServer::AddQueriesStream(
    istream& query_input, ostream& search_results_output
) {
    futures.push_back(async(AddQueriesStreamAsync, ref(query_input),
            ref(search_results_output), ref(index)));
}


void InvertedIndex::Add(istream& documents) {
    for(string document; getline(documents, document); ){
    
  docs.push_back(document);

  const size_t docid = docs.size() - 1;
        auto con = SplitIntoWords(document);
  for (auto word : con) {
      
      auto& docids = index[word];
      
      if (!docids.empty() && docids.back().doc_id_==docid ) {
          docids.back().rating_++;
      } else {
          docids.push_back({docid, 1});
      }
  }
    }
}

vector<InvertedIndex::DocRating> InvertedIndex::Lookup(string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}

