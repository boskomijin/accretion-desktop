/**
 * @file OpenGLRenderer.hpp
 * @brief Defines the OpenGL ES implementation of the IRenderer interface.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

#pragma once

#include "swarcs/accretion/graphics/IRenderer.hpp"
#include "swarcs/accretion/graphics/ShaderManager.hpp"
#include <GLES2/gl2.h>
#include <string>

namespace swarcs::accretion::graphics {

/**
 * @brief OpenGL ES implementation of the IRenderer interface.
 *
 * Manages shader programs, viewport clearing, uniform passing, and coordinates
 * rendering calls for OpenGL-based backends.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */
class OpenGLRenderer : public IRenderer {
private:
    ShaderManager shader; ///< Shader manager handling vertex and fragment programs.

public:
    /**
     * @brief Constructs the OpenGLRenderer with paths to shader source files.
     *
     * @param vertexPath Path to the GLSL vertex shader file.
     * @param fragmentPath Path to the GLSL fragment shader file.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    OpenGLRenderer(const std::string& vertexPath, const std::string& fragmentPath)
        : shader(vertexPath, fragmentPath) {}

    /**
     * @brief Destroys the OpenGLRenderer instance.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    ~OpenGLRenderer() override = default;

    /**
     * @brief Prepares the frame by clearing the color buffer.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    void beginFrame() override {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    /**
     * @brief Binds shader uniforms and triggers scene submission.
     *
     * @param scene Reference to the renderable scene.
     * @param frameContext Current frame timing and resolution data.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    void render(const IRenderable& scene, const FrameContext& frameContext) override {
        shader.use();
        shader.setFloat("u_time", frameContext.time);
        shader.setVec2("u_resolution", static_cast<float>(frameContext.width), static_cast<float>(frameContext.height));

        scene.submit(*this, frameContext);
    }

    /**
     * @brief Finalizes the frame rendering.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    void endFrame() override {
        // Post-frame OpenGL operations if needed
    }
};

} // namespace swarcs::accretion::graphics