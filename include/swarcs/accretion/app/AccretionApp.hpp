#pragma once

#include "swarcs/accretion/platform/IWindow.hpp"
#include "swarcs/accretion/platform/IGraphicsContext.hpp"
#include "swarcs/accretion/graphics/IRenderable.hpp"
#include "swarcs/accretion/graphics/IRenderer.hpp"
#include "swarcs/accretion/graphics/FrameBuffer.hpp"
#include <memory>

namespace swarcs::accretion::app {

    /**
     * @brief Manages the core application lifecycle, window events, and multi-pass rendering loop.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class AccretionApp {
    private:
        platform::IWindow& window;
        platform::IGraphicsContext& graphicsContext;
        std::unique_ptr<graphics::IRenderer> sceneRenderer;
        std::unique_ptr<graphics::IRenderer> bloomRenderer;
        std::unique_ptr<graphics::FrameBuffer> frameBuffer;
        graphics::IRenderable& renderableScene;
        graphics::IRenderable& fullScreenQuad;

    public:
        /**
         * @brief Constructs the AccretionApp instance with injected dependencies for multi-pass rendering.
         *
         * @param windowImpl Reference to the window implementation.
         * @param contextImpl Reference to the graphics context implementation.
         * @param sceneRendererImpl Unique pointer to the main scene renderer implementation.
         * @param bloomRendererImpl Unique pointer to the bloom post-processing renderer implementation.
         * @param frameBufferImpl Unique pointer to the off-screen framebuffer.
         * @param sceneImpl Reference to the renderable scene component.
         * @param quadImpl Reference to the full-screen quad component.
         *
         * @author Bosko Mijin
         * @since 2026-08
         */
        AccretionApp(platform::IWindow& windowImpl,
                     platform::IGraphicsContext& contextImpl,
                     std::unique_ptr<graphics::IRenderer> sceneRendererImpl,
                     std::unique_ptr<graphics::IRenderer> bloomRendererImpl,
                     std::unique_ptr<graphics::FrameBuffer> frameBufferImpl,
                     graphics::IRenderable& sceneImpl,
                     graphics::IRenderable& quadImpl);

        ~AccretionApp() = default;

        // Delete copy operations to ensure unique resource management
        AccretionApp(const AccretionApp&) = delete;
        AccretionApp& operator=(const AccretionApp&) = delete;

        /**
         * @brief Executes the main application lifecycle loop with multi-pass rendering.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        void run() const;
    };

} // namespace swarcs::accretion::app