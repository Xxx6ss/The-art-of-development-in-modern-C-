//
//  json.h
//  Refactoring
//
//  Created by Andrew Kireev on 10.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef json_h
#define json_h

#include <istream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

namespace Json {

class Node {
public:
  explicit Node(std::vector<Node> array);
  explicit Node(std::map<std::string, Node> map);
  explicit Node(int value);
  explicit Node(std::string value);

  const std::vector<Node>& AsArray() const;
  const std::map<std::string, Node>& AsMap() const;
  int AsInt() const;
  const std::string& AsString() const;

private:
  std::vector<Node> as_array;
  std::map<std::string, Node> as_map;
  int as_int;
  std::string as_string;
};

class Document {
public:
  explicit Document(Node root);

  const Node& GetRoot() const;

private:
  Node root;
};

Document Load(std::istream& input);

}

#endif /* json_h */
