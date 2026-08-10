#pragma once

#include <string>

namespace swarcs::accretion::graphics {

/**
 * @brief Manages loading, compilation, linking, and uniform binding for OpenGL shaders.
 *
 * ShaderManager handles source file reading, shader compilation with error diagnostics,
 * program linking, and setting uniform variables used during rendering.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class ShaderManager {
private:
    unsigned int programID; ///< OpenGL handle representing the linked shader program.

    /**
     * @brief Loads shader source code from a specified file path.
     *
     * @param filepath Path to the shader source file.
     * @return std::string The contents of the shader file.
     */
    static std::string loadShaderSource(const std::string& filepath);

    /**
     * @brief Checks for compilation or linking errors in shaders and programs.
     *
     * @param shaderOrProgram OpenGL identifier of the shader or program to check.
     * @param type String descriptor indicating the type ("VERTEX", "FRAGMENT", or "PROGRAM").
     */
    static void checkCompileErrors(unsigned int shaderOrProgram, const std::string& type);

public:
    /**
     * @brief Constructs a ShaderManager by loading and compiling vertex and fragment shaders.
     *
     * @param vertexPath Path to the vertex shader GLSL source file.
     * @param fragmentPath Path to the fragment shader GLSL source file.
     */
    ShaderManager(const std::string& vertexPath, const std::string& fragmentPath);

    /**
     * @brief Destroys the ShaderManager and releases the compiled shader program.
     */
    ~ShaderManager();

    // Delete copy constructor and assignment operator to prevent duplicate resource management
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    /**
     * @brief Activates the shader program for rendering use.
     */
    void use() const;

    /**
     * @brief Sets a float uniform variable in the active shader program.
     *
     * @param name Name of the uniform variable in the GLSL code.
     * @param value Float value to assign.
     */
    void setFloat(const std::string& name, float value) const;

    /**
     * @brief Sets a 2-component vector uniform variable in the active shader program.
     *
     * @param name Name of the uniform variable in the GLSL code.
     * @param x First component value (e.g., width).
     * @param y Second component value (e.g., height).
     */
    void setVec2(const std::string& name, float x, float y) const;
};

} // namespace swarcs::accretion::graphics