#include "ShaderProgramManager.h"

#include "glad/glad.h"

#include <fstream>

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

void ShaderProgramManager::compileProgram(const std::string& programName) {
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

void ShaderProgramManager::setActiveProgram(const std::string& programName) {
    if(programName != activeProgram) {
        glUseProgram(programs.at(programName));
        activeProgram = programName;
    }
}

void ShaderProgramManager::setBool(const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(programs.at(activeProgram), name.c_str()), (int)value);
}

void ShaderProgramManager::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(programs.at(activeProgram), name.c_str()), value);
}

void ShaderProgramManager::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(programs.at(activeProgram), name.c_str()), value);
}
