/**
* @file FullScreenQuad.cpp
 * @brief Implementation of full-screen quad rendering using vertex ID generation.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/graphics/FullScreenQuad.hpp"
#include <GLES3/gl3.h>

namespace swarcs::accretion::graphics {

    /**
     * @brief Constructs the full-screen quad.
     */
    FullScreenQuad::FullScreenQuad() = default;

    /**
     * @brief Destroys the full-screen quad.
     */
    FullScreenQuad::~FullScreenQuad() = default;

    /**
     * @brief Submits the full-screen quad to the renderer for drawing.
     */
    void FullScreenQuad::submit(IRenderer& /*renderer*/, const FrameContext& /*context*/) const {
        // Render a full-screen triangle entirely on the GPU using gl_VertexID without any CPU vertex arrays
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

} // namespace swarcs::accretion::graphics