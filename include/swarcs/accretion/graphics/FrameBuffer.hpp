/**
* @file FrameBuffer.hpp
 * @brief Encapsulates OpenGL Framebuffer Object (FBO) for off-screen rendering.
 *
 * @author Bosko
 * @since 2026-08
 */

#pragma once

#include <GLES3/gl3.h>

namespace swarcs::accretion::graphics {

    class FrameBuffer {
    public:
        FrameBuffer(int width, int height);
        ~FrameBuffer();

        void bind() const;

        static void unbind();

        [[nodiscard]] GLuint getTextureID() const { return colorTexture; }
        void resize(int width, int height);

    private:
        GLuint fbo = 0;
        GLuint colorTexture = 0;
        GLuint depthRenderBuffer = 0;
        int width;
        int height;

        void create(int w, int h);
        void cleanup() const;
    };

} // namespace swarcs::accretion::graphics