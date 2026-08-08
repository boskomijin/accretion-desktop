#pragma once

#include "swarcs/accretion/platform/IWindow.hpp"
#include "swarcs/accretion/platform/IGraphicsContext.hpp"
#include "swarcs/accretion/graphics/ShaderManager.hpp"
#include "swarcs/accretion/graphics/IRenderable.hpp"

namespace swarcs::accretion::app {

    /**
     * @brief Core application manager driving the main loop, window events, and rendering pipeline.
     *
     * AccretionApp coordinates the window system, graphics context, shader program configuration,
     * and polymorphic scene rendering during the application execution lifecycle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class AccretionApp {
    private:
        platform::IWindow& window;               ///< Reference to the platform window.
        platform::IGraphicsContext& graphicsContext; ///< Reference to the graphics context.
        graphics::ShaderManager shader;          ///< Shader manager (owned by App).
        graphics::IRenderable& renderableScene;  ///< Reference to the renderable scene.

    public:
        /**
         * @brief Constructs the application with injected platform, context, and scene dependencies.
         *
         * @param windowImpl Reference to the window implementation.
         * @param contextImpl Reference to the graphics context implementation.
         * @param sceneImpl Reference to the renderable scene component.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        AccretionApp(platform::IWindow& windowImpl,
                     platform::IGraphicsContext& contextImpl,
                     graphics::IRenderable& sceneImpl);

        /**
         * @brief Defaulted destructor.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        ~AccretionApp() = default;

        /**
         * @brief Starts and runs the main event and rendering loop.
         */
        void run();
    };

} // namespace swarcs::accretion::app