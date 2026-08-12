/**
 * @file AccretionApp.cpp
 * @brief Implementation of AccretionApp managing multi-pass rendering (Scene + Bloom Post-Processing).
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/app/AccretionApp.hpp"
#include "swarcs/accretion/graphics/FrameBuffer.hpp"
#include <chrono>
#include <memory>

namespace swarcs::accretion::app {

    /**
     * @brief Constructs the AccretionApp instance with injected dependencies and post-processing pipeline.
     *
     * @param windowImpl Reference to the window implementation.
     * @param contextImpl Reference to the graphics context implementation.
     * @param sceneRendererImpl Unique pointer to the main scene renderer.
     * @param bloomRendererImpl Unique pointer to the bloom post-processing renderer.
     * @param frameBufferImpl Unique pointer to the off-screen framebuffer.
     * @param sceneImpl Reference to the renderable scene component.
     * @param quadImpl Reference to the full-screen quad for post-processing.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    AccretionApp::AccretionApp(platform::IWindow& windowImpl,
                               platform::IGraphicsContext& contextImpl,
                               std::unique_ptr<graphics::IRenderer> sceneRendererImpl,
                               std::unique_ptr<graphics::IRenderer> bloomRendererImpl,
                               std::unique_ptr<graphics::FrameBuffer> frameBufferImpl,
                               graphics::IRenderable& sceneImpl,
                               graphics::IRenderable& quadImpl)
        : window(windowImpl),
          graphicsContext(contextImpl),
          sceneRenderer(std::move(sceneRendererImpl)),
          bloomRenderer(std::move(bloomRendererImpl)),
          frameBuffer(std::move(frameBufferImpl)),
          renderableScene(sceneImpl),
          fullScreenQuad(quadImpl) {}

    /**
     * @brief Executes the main application lifecycle loop with multi-pass rendering.
     *
     * Pass 1: Renders the scene into an off-screen Framebuffer texture.
     * Pass 2: Applies the bloom post-processing shader over the full-screen quad and presents the frame.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void AccretionApp::run() const {
        const auto startTime = std::chrono::steady_clock::now();
        auto lastFrameTime = startTime;

        while (true) {
            window.processEvents(); // Flush X11 event queue

            const auto currentTime = std::chrono::steady_clock::now();

            graphics::FrameContext frameContext;
            frameContext.time = std::chrono::duration<float>(currentTime - startTime).count();
            frameContext.deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
            frameContext.width = window.getWidth();
            frameContext.height = window.getHeight();

            lastFrameTime = currentTime;

            // --- PASS 1: Render main scene into off-screen Framebuffer ---
            frameBuffer->bind();
            sceneRenderer->beginFrame();
            sceneRenderer->render(renderableScene, frameContext);
            sceneRenderer->endFrame();
            frameBuffer->unbind();

            // --- PASS 2: Post-processing & Bloom over screen ---
            bloomRenderer->beginFrame();
            // Texture from frameBuffer->getTextureID() is passed into the bloom shader (uSceneTexture)
            bloomRenderer->render(fullScreenQuad, frameContext);
            bloomRenderer->endFrame();

            graphicsContext.swapBuffers();
        }
    }

} // namespace swarcs::accretion::app