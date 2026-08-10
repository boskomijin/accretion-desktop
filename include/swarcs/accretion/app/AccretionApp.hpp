#pragma once

#include "swarcs/accretion/platform/IWindow.hpp"
#include "swarcs/accretion/platform/IGraphicsContext.hpp"
#include "swarcs/accretion/graphics/IRenderable.hpp"
#include "swarcs/accretion/graphics/IRenderer.hpp"
#include <memory>

namespace swarcs::accretion::app {

    /**
     * @brief Manages the core application lifecycle, window events, and rendering loop.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class AccretionApp {
    private:
        platform::IWindow& window;
        platform::IGraphicsContext& graphicsContext;
        std::unique_ptr<graphics::IRenderer> renderer;
        graphics::IRenderable& renderableScene;

    public:
        /**
         * @brief Constructs the AccretionApp instance with injected dependencies.
         *
         * @param windowImpl Reference to the window implementation.
         * @param contextImpl Reference to the graphics context implementation.
         * @param rendererImpl Unique pointer to the graphics renderer implementation.
         * @param sceneImpl Reference to the renderable scene component.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        AccretionApp(platform::IWindow& windowImpl,
                     platform::IGraphicsContext& contextImpl,
                     std::unique_ptr<graphics::IRenderer> rendererImpl,
                     graphics::IRenderable& sceneImpl);

        ~AccretionApp() = default;

        // Delete copy operations to ensure unique resource management
        AccretionApp(const AccretionApp&) = delete;
        AccretionApp& operator=(const AccretionApp&) = delete;

        /**
         * @brief Executes the main application lifecycle loop.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        void run() const;
    };

} // namespace swarcs::accretion::app