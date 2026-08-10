/**
 * @file ShaderManager.cpp
 * @brief Implementation of the ShaderManager class for loading, compiling, and linking shaders.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/graphics/ShaderManager.hpp"
#include "swarcs/accretion/graphics/ShaderHandle.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace swarcs::accretion::graphics {

/**
 * @brief Loads shader source code from a specified file path.
 *
 * @param filepath Path to the shader source file.
 * @return std::string The contents of the shader file.
 */
std::string ShaderManager::loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Checks for compilation or linking errors in shaders and programs.
 *
 * @param shaderOrProgram OpenGL identifier of the shader or program to check.
 * @param type String descriptor indicating the type ("VERTEX", "FRAGMENT", or "PROGRAM").
 */
void ShaderManager::checkCompileErrors(unsigned int shaderOrProgram, const std::string& type) {
    int success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shaderOrProgram, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderOrProgram, 1024, nullptr, infoLog);
            throw std::runtime_error("SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
        }
    } else {
        glGetProgramiv(shaderOrProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderOrProgram, 1024, nullptr, infoLog);
            throw std::runtime_error("PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog);
        }
    }
}

/**
 * @brief Constructs a ShaderManager by loading, compiling, and linking shaders using RAII.
 *
 * @param vertexPath Path to the vertex shader GLSL source file.
 * @param fragmentPath Path to the fragment shader GLSL source file.
 */
ShaderManager::ShaderManager(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // RAII wrappers guarantee exception safety and zero resource leaks on failure
    ShaderHandle vertexShader(GL_VERTEX_SHADER);
    ShaderHandle fragmentShader(GL_FRAGMENT_SHADER);
    ProgramHandle compiledProgram;

    glShaderSource(vertexShader.get(), 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader.get());
    checkCompileErrors(vertexShader.get(), "VERTEX");

    glShaderSource(fragmentShader.get(), 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader.get());
    checkCompileErrors(fragmentShader.get(), "FRAGMENT");

    glAttachShader(compiledProgram.get(), vertexShader.get());
    glAttachShader(compiledProgram.get(), fragmentShader.get());
    glLinkProgram(compiledProgram.get());
    checkCompileErrors(compiledProgram.get(), "PROGRAM");

    // Move ownership directly into member variable — no raw release/reassign needed
    program = std::move(compiledProgram);
}

/**
 * @brief Activates the shader program for rendering use.
 */
void ShaderManager::use() const {
    glUseProgram(program.get());
}

/**
 * @brief Sets a float uniform variable in the active shader program.
 *
 * @param name Name of the uniform variable in the GLSL code.
 * @param value Float value to assign.
 */
void ShaderManager::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(program.get(), name.c_str()), value);
}

/**
 * @brief Sets a 2-component vector uniform variable in the active shader program.
 *
 * @param name Name of the uniform variable in the GLSL code.
 * @param x First component value (e.g., width).
 * @param y Second component value (e.g., height).
 */
void ShaderManager::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program.get(), name.c_str()), x, y);
}

} // namespace swarcs::accretion::graphics