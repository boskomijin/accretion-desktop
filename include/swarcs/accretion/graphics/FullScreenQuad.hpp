#pragma once

#include "swarcs/accretion/graphics/IRenderable.hpp"

namespace swarcs::accretion::graphics {

    /**
     * @brief Represents a full-screen quad geometry used for post-processing and shader backgrounds.
     *
     * FullScreenQuad renders a procedural full-screen triangle using gl_VertexID,
     * completely eliminating the need for Vertex Buffer Objects (VBOs) or vertex attributes.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class FullScreenQuad : public IRenderable {
    public:
        /**
         * @brief Constructs the full-screen quad.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        FullScreenQuad();

        /**
         * @brief Destroys the full-screen quad.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        ~FullScreenQuad() override;

        // Delete copy operations
        FullScreenQuad(const FullScreenQuad&) = delete;
        FullScreenQuad& operator=(const FullScreenQuad&) = delete;

        /**
         * @brief Issues a draw call for the procedural full-screen triangle.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        void draw() const override;
    };

} // namespace swarcs::accretion::graphics