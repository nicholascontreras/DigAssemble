#pragma once

#include <string>
#include <unordered_map>

class ShaderProgramManager {
public:
    static void compileProgram(const std::string& programName);
    static void setActiveProgram(const std::string& programName);

    static void setBool(const std::string& name, bool value);
    static void setInt(const std::string& name, int value);
    static void setFloat(const std::string& name, float value);
    
private:
    static std::unordered_map<std::string, unsigned int> programs;
    static std::string activeProgram;

    static unsigned int compileShader(const std::string& shaderFile);
};

