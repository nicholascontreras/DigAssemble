#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class ShaderProgramManager {
public:
    static void compileProgram(const std::string& programName);
    static void setActiveProgram(const std::string& programName);

    static void setBool(const std::string& name, const bool& value);
    static void setInt(const std::string& name, const int& value);
    static void setFloat(const std::string& name, const float& value);
    static void setMat4(const std::string& name, const glm::mat4& value);
    static void setVec3(const std::string& name, const glm::vec3& value);
    
private:
    static std::unordered_map<std::string, unsigned int> programs;
    static std::string activeProgram;

    static unsigned int compileShader(const std::string& shaderFile);
    static unsigned int getUniformLocation(const std::string& name);
};

