#include "ShaderProgramManager.h"

#include <glad/glad.h>

#include <fstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

std::unordered_map<std::string, unsigned int> ShaderProgramManager::programs;
std::string ShaderProgramManager::activeProgram;

unsigned int ShaderProgramManager::compileShader(const std::string& shaderFile) {

    std::string shaderSource;
    std::fstream inputFile("shaders/" + shaderFile);
    if(inputFile.is_open()) {
        char curChar;
        while(inputFile.get(curChar)) {
            shaderSource += curChar;
        }
        inputFile.close();
    }

    const char* sharedSourceCStr = shaderSource.c_str();

    bool isVertexShader = shaderFile.find(".vert") != std::string::npos;
    
    unsigned int shaderId = glCreateShader(isVertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(shaderId, 1, &sharedSourceCStr, NULL);
    glCompileShader(shaderId);

    return shaderId;
}

void ShaderProgramManager::setActiveProgram(const std::string& programName) {
    if(programName != activeProgram) {
        glUseProgram(programs.at(programName));
        activeProgram = programName;
    }
}

void ShaderProgramManager::setBool(const std::string& name, const bool& value) {
    setInt(name, (int)value);
}

void ShaderProgramManager::setInt(const std::string& name, const int& value) {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgramManager::setFloat(const std::string& name, const float& value) {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgramManager::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(value));
}

void ShaderProgramManager::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgramManager::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgramManager::compileProgram(const std::string& programName) {
    if(programs.count(programName)) {
        throw std::invalid_argument("The given program name is already compiled!");
    }

    unsigned int vertShaderId = compileShader(programName + ".vert");
    unsigned int fragShaderId = compileShader(programName + ".frag");

    unsigned int programId;
    programId = glCreateProgram();

    glAttachShader(programId, vertShaderId);
    glAttachShader(programId, fragShaderId);

    glLinkProgram(programId);

    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);

    programs.emplace(programName, programId);
}

unsigned int ShaderProgramManager::getUniformLocation(const std::string& name) {
    return glGetUniformLocation(programs.at(activeProgram), name.c_str());
}
