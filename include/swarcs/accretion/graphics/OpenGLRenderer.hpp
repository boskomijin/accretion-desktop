#pragma once

#include "swarcs/accretion/graphics/IRenderer.hpp"
#include "swarcs/accretion/graphics/ShaderManager.hpp"
#include <GLES2/gl2.h>
#include <string>

namespace swarcs::accretion::graphics {

    /**
     * @brief OpenGL ES implementation of the IRenderer interface.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    class OpenGLRenderer : public IRenderer {
    private:
        ShaderManager shader;

    public:
        OpenGLRenderer(const std::string& vertexPath, const std::string& fragmentPath)
            : shader(vertexPath, fragmentPath) {}

        ~OpenGLRenderer() override = default;

        void beginFrame() override {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void render(const IRenderable& scene, const FrameContext& frameContext) override {
            shader.use();
            shader.setFloat("u_time", frameContext.time);
            shader.setVec2("u_resolution", static_cast<float>(frameContext.width), static_cast<float>(frameContext.height));

            scene.draw();
        }

        void endFrame() override {
            // OpenGL specific post-frame operations if required
        }
    };

} // namespace swarcs::accretion::graphics