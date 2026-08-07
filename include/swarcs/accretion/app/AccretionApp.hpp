#pragma once

#include "swarcs/accretion/platform/IWindow.hpp"
#include "swarcs/accretion/platform/IGraphicsContext.hpp"
#include "swarcs/accretion/graphics/ShaderManager.hpp"
#include "swarcs/accretion/graphics/IRenderable.hpp"
#include <memory>

namespace swarcs::accretion::app {

/**
 * @brief Core application manager driving the main loop, window events, and rendering pipeline.
 *
 * AccretionApp coordinates the window system, graphics context, shader program configuration,
 * and polimorphic scene rendering during the application execution lifecycle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class AccretionApp {
private:
    std::unique_ptr<platform::IWindow> window;               ///< Abstract interface instance managing platform window operations.
    std::unique_ptr<platform::IGraphicsContext> graphicsContext; ///< Abstract graphics context managing buffer swapping and presentation.

    graphics::ShaderManager shader;                          ///< Shader manager handling vertex and fragment shader compilation and binding.

    std::unique_ptr<graphics::IRenderable> renderableScene;  ///< Polymorphic renderable scene component displayed on the canvas.

public:
    /**
     * @brief Constructs the application with injected platform, context, and scene dependencies.
     *
     * @param windowImpl Unique pointer to the window implementation.
     * @param contextImpl Unique pointer to the graphics context implementation.
     * @param sceneImpl Unique pointer to the renderable scene component.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    AccretionApp(std::unique_ptr<platform::IWindow> windowImpl,
                 std::unique_ptr<platform::IGraphicsContext> contextImpl,
                 std::unique_ptr<graphics::IRenderable> sceneImpl);

    /**
     * @brief Defaulted destructor releasing managed application resources.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    ~AccretionApp() = default;

    /**
     * @brief Starts and runs the main event and rendering loop.
     *
     * Continuously processes window events, tracks elapsed time, updates shader uniforms,
     * clears buffers, renders the active scene, and swaps graphics buffers until exit.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void run();
};

} // namespace swarcs::accretion::app