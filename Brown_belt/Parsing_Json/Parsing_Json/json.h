//
//  json.h
//  Parsing_Json
//
//  Created by Andrew Kireev on 09.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef json_h
#define json_h

#pragma once

#include <istream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
using namespace std;

class Node {
public:
  explicit Node(vector<Node> array);
  explicit Node(map<string, Node> map);
  explicit Node(int value);
  explicit Node(string value);

  const vector<Node>& AsArray() const;
  const map<string, Node>& AsMap() const;
  int AsInt() const;
  const string& AsString() const;

private:
  vector<Node> as_array;
  map<string, Node> as_map;
  int as_int;
  string as_string;
};

class Document {
public:
  explicit Document(Node root);

  const Node& GetRoot() const;

private:
  Node root;
};

Document Load(istream& input);


#endif /* json_h */
