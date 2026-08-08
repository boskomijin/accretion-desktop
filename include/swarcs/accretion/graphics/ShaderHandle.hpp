#pragma once

namespace swarcs::accretion::graphics {

    /**
     * @brief RAII wrapper for individual OpenGL shader objects (vertex or fragment).
     *
     * Automatically handles shader object creation and ensures safe deletion via glDeleteShader
     * upon destruction if ownership is not released.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class ShaderHandle {
    private:
        unsigned int id{0}; ///< Raw OpenGL shader identifier.

    public:
        /**
         * @brief Constructs and creates an OpenGL shader object of the specified type.
         *
         * @param shaderType GLenum representing the shader type (e.g., GL_VERTEX_SHADER).
         * @throws std::runtime_error if shader creation fails.
         */
        explicit ShaderHandle(unsigned int shaderType);

        /**
         * @brief Destructor releasing the OpenGL shader resource if still owned.
         */
        ~ShaderHandle();

        // Delete copy operations to enforce strict RAII ownership
        ShaderHandle(const ShaderHandle&) = delete;
        ShaderHandle& operator=(const ShaderHandle&) = delete;

        // Enable move semantics for safe transfer of ownership
        ShaderHandle(ShaderHandle&& other) noexcept;
        ShaderHandle& operator=(ShaderHandle&& other) noexcept;

        /**
         * @brief Retrieves the raw OpenGL shader identifier.
         * @return unsigned int The shader ID.
         */
        unsigned int get() const;

        /**
         * @brief Releases ownership of the shader ID without deleting it.
         * @return unsigned int The released shader ID.
         */
        unsigned int release();
    };

} // namespace swarcs::accretion::graphics