/**
* @file ProgramHandle.cpp
 * @brief Implementation of the ProgramHandle RAII wrapper class.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/graphics/ProgramHandle.hpp"
#include <GLES2/gl2.h>
#include <stdexcept>

namespace swarcs::accretion::graphics {

    /**
     * @brief Constructs and creates an OpenGL shader program object.
     * @throws std::runtime_error if program creation fails.
     */
    ProgramHandle::ProgramHandle() : id(glCreateProgram()) {
        if (id == 0) {
            throw std::runtime_error("Failed to create shader program object.");
        }
    }

    /**
     * @brief Destructor releasing the OpenGL program resource if still owned.
     */
    ProgramHandle::~ProgramHandle() {
        if (id != 0) {
            glDeleteProgram(id);
        }
    }

    /**
     * @brief Move constructor transferring ownership of the program handle.
     *
     * @param other Rvalue reference to another ProgramHandle instance.
     */
    ProgramHandle::ProgramHandle(ProgramHandle&& other) noexcept : id(other.id) {
        other.id = 0;
    }

    /**
     * @brief Move assignment operator transferring ownership of the program handle.
     *
     * @param other Rvalue reference to another ProgramHandle instance.
     * @return ProgramHandle& Reference to this assigned instance.
     */
    ProgramHandle& ProgramHandle::operator=(ProgramHandle&& other) noexcept {
        if (this != &other) {
            if (id != 0) {
                glDeleteProgram(id);
            }
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    /**
     * @brief Retrieves the raw OpenGL program identifier.
     * @return unsigned int The program ID.
     */
    unsigned int ProgramHandle::get() const {
        return id;
    }

    /**
     * @brief Releases ownership of the program ID without deleting it.
     * @return unsigned int The released program ID.
     */
    unsigned int ProgramHandle::release() {
        unsigned int temp = id;
        id = 0;
        return temp;
    }

} // namespace swarcs::accretion::graphics