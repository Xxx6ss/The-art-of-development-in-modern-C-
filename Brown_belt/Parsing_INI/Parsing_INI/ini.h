//
//  ini.h
//  Parsing_INI
//
//  Created by Andrew Kireev on 10.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef ini_h
#define ini_h

#include <unordered_map>
#include <string>

namespace Ini {

using Section = std::unordered_map<std::string, std::string>;

class Document {
public:
  Section& AddSection(std::string name);
  const Section& GetSection(const std::string& name) const;
  size_t SectionCount() const;

private:
  std::unordered_map<std::string, Section> sections;
};

Document Load(std::istream& input);

}

#endif /* ini_h */
