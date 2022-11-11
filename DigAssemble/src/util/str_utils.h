#pragma once

#include <string>
#include <vector>

inline std::vector<std::string> split(const std::string& source, const std::string& delim, bool ignoreEmptyLast = true) {
    std::vector<std::string> splitString;

    size_t startIndex = 0;
    while(true) {
        size_t delimIndex = source.find(delim, startIndex);
        if(delimIndex == std::string::npos) {
            break;
        }

        splitString.push_back(source.substr(startIndex, delimIndex - startIndex));
        startIndex = delimIndex + delim.length();
    }

    if(!source.substr(startIndex).empty() || !ignoreEmptyLast) {
        splitString.push_back(source.substr(startIndex));
    }
    return splitString;
}
