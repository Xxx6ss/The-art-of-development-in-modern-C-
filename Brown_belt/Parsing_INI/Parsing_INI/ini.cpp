//
//  ini.cpp
//  Parsing_INI
//
//  Created by Andrew Kireev on 10.08.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include "ini.h"
#include <istream>

namespace Ini {

using namespace std;

Section& Document::AddSection(std::string name) {
    if (!sections.count(name)) {
        sections[name] = {};
    }
    return sections[name];
}

const Section& Document::GetSection(const std::string& name) const {
    return sections.at(name);
}

size_t Document::SectionCount() const {
    return sections.size();
}

Document Load(std::istream& input) {
    std::string line;
    std::string name;
    Document doc;
    Section* sec_ptr = nullptr;
    
    while ((getline(input, line))) {
        while (!line.empty() && (line.back() == ' ' || line.back() == '\n' || line.back() == '\t')) {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }
        if (line[0] == '[' && line.back() == ']') {
            name = line;
            name.erase(name.begin());
            name.erase(name.find(']'));
            sec_ptr = &doc.AddSection(name);
        }
         else {
            auto it = line.find('=');
            sec_ptr->insert({ line.substr(0, it),
                line.substr(it + 1,(int)line.size() - it - 1) });
         }
    }
    return doc;
}

}
