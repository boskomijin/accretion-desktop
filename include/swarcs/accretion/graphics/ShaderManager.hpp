/**
 * @file ShaderManager.hpp
 * @brief Manages compilation, linking, and uniform configuration for OpenGL ES shader programs.
 *
 * ShaderManager loads vertex and fragment shaders from disk, compiles them safely,
 * handles program linking, and provides encapsulated methods for binding and uniform updates.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

#include <string>
#include <string_view>
#include <GLES2/gl2.h>

namespace swarcs::accretion::graphics {

/**
 * @brief RAII wrapper for OpenGL ES shader programs and pipeline configuration.
 *
 * Encapsulates raw shader object IDs, file loading logic, compilation error handling,
 * and uniform setter operations.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class ShaderManager {
private:
    GLuint programID = 0; ///< Handle to the linked OpenGL shader program.

    /**
     * @brief Loads raw shader source code from a file on disk.
     *
     * @param filepath Path to the shader source file.
     * @return std::string File content as a string.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    std::string loadShaderFile(std::string_view filepath);

    /**
     * @brief Compiles an individual shader stage of a given type.
     *
     * @param type OpenGL shader type (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
     * @param source Source code string of the shader.
     * @return GLuint Handle to the compiled shader object.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    GLuint compileShader(GLenum type, std::string_view source);

public:
    /**
     * @brief Constructs and initializes a shader manager by loading and linking vertex and fragment shaders.
     *
     * @param vertexPath Filepath to the vertex shader source.
     * @param fragmentPath Filepath to the fragment shader source.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    ShaderManager(std::string_view vertexPath, std::string_view fragmentPath);

    /**
     * @brief Destroys the shader manager and frees the OpenGL program resource.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    ~ShaderManager();

    // Delete copy operations to maintain unique resource ownership under RAII
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    /**
     * @brief Binds the shader program for use in the rendering pipeline.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void use() const;

    /**
     * @brief Returns the raw OpenGL program identifier.
     *
     * @return GLuint Program handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    GLuint getID() const;

    /**
     * @brief Sets a floating-point uniform variable in the shader program.
     *
     * @param name Name of the uniform variable in GLSL source.
     * @param value Floating-point value to assign.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void setFloat(std::string_view name, float value) const;

    /**
     * @brief Sets a 2D vector uniform variable in the shader program.
     *
     * @param name Name of the uniform variable in GLSL source.
     * @param x X component of the vector.
     * @param y Y component of the vector.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void setVec2(std::string_view name, float x, float y) const;
};

} // namespace swarcs::accretion::graphics