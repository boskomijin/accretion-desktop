#include "swarcs/accretion/app/AccretionApp.hpp"
#include <chrono>

namespace swarcs::accretion::app {

/**
 * @brief Constructs the AccretionApp instance and initializes core subsystems.
 *
 * @param windowImpl Unique pointer to the window implementation.
 * @param contextImpl Unique pointer to the graphics context implementation.
 * @param sceneImpl Unique pointer to the renderable scene component.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
AccretionApp::AccretionApp(std::unique_ptr<platform::IWindow> windowImpl,
                           std::unique_ptr<platform::IGraphicsContext> contextImpl,
                           std::unique_ptr<graphics::IRenderable> sceneImpl)
    : window(std::move(windowImpl)),
      graphicsContext(std::move(contextImpl)),
      shader("shaders/vertex.glsl", "shaders/fragment.glsl"),
      renderableScene(std::move(sceneImpl)) {}

/**
 * @brief Executes the main application lifecycle loop.
 *
 * Tracks real-time elapsed duration for animations, updates uniform variables
 * (such as time and resolution) in the shader program, performs frame clearing,
 * triggers polymorphic rendering, and presents frames via buffer swapping.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void AccretionApp::run() {
    // Record the starting point of the application execution time
    auto startTime = std::chrono::high_resolution_clock::now();

    // Loop continuously while window events are processed and window remains open
    while (window->processEvents()) {
        // Calculate elapsed time from start for shader time-based animations
        auto currentTime = std::chrono::high_resolution_clock::now();
        float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

        // Bind the shader program for the current frame
        shader.use();

        // Pass dynamic uniforms to the active shader program
        shader.setFloat("u_time", elapsedTime);
        shader.setVec2("u_resolution", static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()));

        // Clear the color buffer with pure black background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the scene polymorphically if available
        if (renderableScene) {
            renderableScene->draw();
        }

        // Swap the front and back buffers to present the rendered frame
        graphicsContext->swapBuffers();
    }
}

} // namespace swarcs::accretion::app