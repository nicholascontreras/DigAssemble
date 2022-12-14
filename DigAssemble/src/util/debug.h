#pragma once

#include <Windows.h>
#include <string>
#include <stdexcept>

inline void Debug(const char* text, bool newLine = true) {
    OutputDebugString(text);
    if(newLine) {
        OutputDebugString("\r\n");
    }
}

inline void Debug(const unsigned char* text, bool newLine = true) {
    Debug((char*)text, newLine);
}

inline void Debug(const std::string& text, bool newLine = true) {
    Debug(text.c_str(), newLine);
}

inline std::string to_string(const char text[]) {
    // This function is being called from a variadic template function
    // Perform a basic check that the sequenece of chars given is "probably" a string literal
    for(int i = 0; i < 64; i++) {
        if(text[i] == 0) {
            return std::string(text);
        } else if(text[i] < 32 || text[i] > 126) {
            throw std::invalid_argument("Invalid character in text!");
        }
    }
    throw std::invalid_argument("No null terminator found!");
}

template <class T>
inline void Debug(const T& x) {
    using std::to_string;
    using ::to_string;
    Debug(to_string(x));
}

template <class T, class... U>
inline void Debug(const T& x, const U&... y) {
    using std::to_string;
    using ::to_string;
    Debug(to_string(x), false);
    Debug(y...);
}
