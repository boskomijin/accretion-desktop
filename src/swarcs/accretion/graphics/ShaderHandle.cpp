/**
* @file ShaderHandle.cpp
 * @brief Implementation of the ShaderHandle RAII wrapper class.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/graphics/ShaderHandle.hpp"
#include <GLES2/gl2.h>
#include <stdexcept>

namespace swarcs::accretion::graphics {

    /**
     * @brief Constructs and creates an OpenGL shader object of the specified type.
     *
     * @param shaderType GLenum representing the shader type (e.g., GL_VERTEX_SHADER).
     * @throws std::runtime_error if shader creation fails.
     */
    ShaderHandle::ShaderHandle(unsigned int shaderType) : id(glCreateShader(shaderType)) {
        if (id == 0) {
            throw std::runtime_error("Failed to create shader object.");
        }
    }

    /**
     * @brief Destructor releasing the OpenGL shader resource if still owned.
     */
    ShaderHandle::~ShaderHandle() {
        if (id != 0) {
            glDeleteShader(id);
        }
    }

    /**
     * @brief Move constructor transferring ownership of the shader handle.
     *
     * @param other Rvalue reference to another ShaderHandle instance.
     */
    ShaderHandle::ShaderHandle(ShaderHandle&& other) noexcept : id(other.id) {
        other.id = 0;
    }

    /**
     * @brief Move assignment operator transferring ownership of the shader handle.
     *
     * @param other Rvalue reference to another ShaderHandle instance.
     * @return ShaderHandle& Reference to this assigned instance.
     */
    ShaderHandle& ShaderHandle::operator=(ShaderHandle&& other) noexcept {
        if (this != &other) {
            if (id != 0) {
                glDeleteShader(id);
            }
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    /**
     * @brief Retrieves the raw OpenGL shader identifier.
     * @return unsigned int The shader ID.
     */
    unsigned int ShaderHandle::get() const {
        return id;
    }

    /**
     * @brief Releases ownership of the shader ID without deleting it.
     * @return unsigned int The released shader ID.
     */
    unsigned int ShaderHandle::release() {
        unsigned int temp = id;
        id = 0;
        return temp;
    }

} // namespace swarcs::accretion::graphics