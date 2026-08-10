/**
 * @file FullScreenQuad.cpp
 * @brief Implementation of full-screen quad rendering using client-side vertex arrays.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/graphics/FullScreenQuad.hpp"
#include <GLES2/gl2.h>

namespace swarcs::accretion::graphics {

    FullScreenQuad::FullScreenQuad() = default;

    FullScreenQuad::~FullScreenQuad() = default;

    /**
     * @brief Issues a draw call for the full-screen triangle using a client-side vertex array.
     */
    void FullScreenQuad::draw() const {
        static const GLfloat vertices[] = {
            -1.0f, -1.0f,
             3.0f, -1.0f,
            -1.0f,  3.0f
        };

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
    }

} // namespace swarcs::accretion::graphics