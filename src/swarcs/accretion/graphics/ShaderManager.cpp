/**
 * @file ShaderManager.cpp
 * @brief Implementation of the ShaderManager class for OpenGL ES resource handling.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/graphics/ShaderManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace swarcs::accretion::graphics {

/**
 * @brief Constructs the ShaderManager by compiling stages and linking the program.
 *
 * @param vertexPath Filepath to vertex shader.
 * @param fragmentPath Filepath to fragment shader.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
ShaderManager::ShaderManager(std::string_view vertexPath, std::string_view fragmentPath) {
    // Load source code for both shader stages from disk
    std::string vertexCode = loadShaderFile(vertexPath);
    std::string fragmentCode = loadShaderFile(fragmentPath);

    // Compile individual shader modules
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    // Create program container, attach shaders, and link
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // Verify program linking status
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        throw std::runtime_error(std::string("Failed to link shader program:\n") + infoLog);
    }

    // Clean up detached individual shader objects after successful linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

/**
 * @brief Destroys the shader manager, deleting the OpenGL program resource if allocated.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
ShaderManager::~ShaderManager() {
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

/**
 * @brief Activates the shader program in the current rendering context.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void ShaderManager::use() const {
    glUseProgram(programID);
}

/**
 * @brief Retrieves the raw OpenGL program ID handle.
 *
 * @return GLuint Program ID.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
GLuint ShaderManager::getID() const {
    return programID;
}

/**
 * @brief Updates a scalar float uniform variable.
 *
 * @param name Uniform identifier string.
 * @param value Floating-point value.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void ShaderManager::setFloat(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.data()), value);
}

/**
 * @brief Updates a 2D vector uniform variable.
 *
 * @param name Uniform identifier string.
 * @param x X component.
 * @param y Y component.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void ShaderManager::setVec2(std::string_view name, float x, float y) const {
    glUniform2f(glGetUniformLocation(programID, name.data()), x, y);
}

/**
 * @brief Reads shader file contents from storage into a string buffer.
 *
 * @param filepath Path of the target file.
 * @return std::string Entire source file content.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
std::string ShaderManager::loadShaderFile(std::string_view filepath) {
    // Uniform initialization {} used to prevent Most Vexing Parse ambiguity
    std::ifstream file{std::string(filepath)};
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open shader file: ") + std::string(filepath));
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Compiles source code into a target OpenGL shader object with error logging.
 *
 * @param type Shader type enum (e.g. GL_VERTEX_SHADER).
 * @param source Source code text.
 * @return GLuint Compiled shader handle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
GLuint ShaderManager::compileShader(GLenum type, std::string_view source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.data();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Verify compilation status
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        throw std::runtime_error(std::string("Failed to compile " + typeStr + " shader:\n") + infoLog);
    }

    return shader;
}

} // namespace swarcs::accretion::graphics