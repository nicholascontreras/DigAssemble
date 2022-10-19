#pragma once

#include <Windows.h>

inline void Debug(const char* text, bool newLine = true) {
    OutputDebugString(text);
    if(newLine) {
        OutputDebugString("\r\n");
    }
}

inline void Debug(const unsigned char* text, bool newLine = true) {
    Debug((char*)text, newLine);
}
