#include "swarcs/accretion/app/AccretionApp.hpp"
#include <GLES2/gl2.h>
#include <chrono>

namespace swarcs::accretion::app {

/**
 * @brief Constructs the AccretionApp instance with injected dependencies.
 *
 * @param windowImpl Reference to the window implementation.
 * @param contextImpl Reference to the graphics context implementation.
 * @param sceneImpl Reference to the renderable scene component.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
AccretionApp::AccretionApp(platform::IWindow& windowImpl,
                           platform::IGraphicsContext& contextImpl,
                           graphics::IRenderable& sceneImpl)
    : window(windowImpl),
      graphicsContext(contextImpl),
      shader("shaders/vertex.glsl", "shaders/fragment.glsl"),
      renderableScene(sceneImpl) {}

/**
 * @brief Executes the main application lifecycle loop.
 *
 * Tracks monotonic elapsed duration for animations, updates uniform variables
 * (such as time and resolution) in the shader program, performs frame clearing,
 * triggers polymorphic rendering, and presents frames via buffer swapping.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void AccretionApp::run() {
    // Record the starting point of the monotonic execution time
    const auto startTime = std::chrono::steady_clock::now();

    // Loop continuously while window events are processed and window remains open
    while (window.processEvents()) {
        // Calculate elapsed time from start using steady_clock for safe animation timing
        const auto currentTime = std::chrono::steady_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        // Bind the shader program for the current frame
        shader.use();

        // Pass dynamic uniforms to the active shader program
        shader.setFloat("u_time", elapsedTime);
        shader.setVec2("u_resolution", static_cast<float>(window.getWidth()), static_cast<float>(window.getHeight()));

        // Clear the color buffer with pure black background (#000000)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the scene polymorphically
        renderableScene.draw();

        // Swap the front and back buffers to present the rendered frame
        graphicsContext.swapBuffers();
    }
}

} // namespace swarcs::accretion::app