#pragma once

#include "swarcs/accretion/graphics/IRenderable.hpp"
#include <GLES2/gl2.h>

namespace swarcs::accretion::graphics {

    /**
     * @brief Represents a full-screen quad geometry used for post-processing and shader backgrounds.
     *
     * FullScreenQuad generates and manages a Vertex Buffer Object (VBO) containing
     * two triangles covering normalized device coordinates (-1 to 1) for rendering wallpaper shaders.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class FullScreenQuad : public IRenderable {
    private:
        GLuint vbo = 0; ///< Vertex Buffer Object handle storing quad vertex positions.

    public:
        /**
         * @brief Constructs the full-screen quad and initializes vertex buffer data.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        FullScreenQuad();

        /**
         * @brief Destroys the full-screen quad and releases GPU buffer resources.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        ~FullScreenQuad() override;

        // Delete copy operations to ensure unique ownership of the underlying GPU buffer resource
        FullScreenQuad(const FullScreenQuad&) = delete;
        FullScreenQuad& operator=(const FullScreenQuad&) = delete;

        /**
         * @brief Binds the vertex buffer attributes and issues a draw call for the quad.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        void draw() const override;
    };

} // namespace swarcs::accretion::graphics