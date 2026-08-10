#include "swarcs/accretion/app/AccretionApp.hpp"
#include <chrono>

namespace swarcs::accretion::app {

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
    AccretionApp::AccretionApp(platform::IWindow& windowImpl,
                               platform::IGraphicsContext& contextImpl,
                               std::unique_ptr<graphics::IRenderer> rendererImpl,
                               graphics::IRenderable& sceneImpl)
        : window(windowImpl),
          graphicsContext(contextImpl),
          renderer(std::move(rendererImpl)),
          renderableScene(sceneImpl) {}

    /**
     * @brief Executes the main application lifecycle loop.
     *
     * Tracks monotonic elapsed duration, populates the frame context, delegates
     * rendering to the injected renderer abstraction, and presents frames.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void AccretionApp::run() const {
        const auto startTime = std::chrono::steady_clock::now();
        auto lastFrameTime = startTime;

        while (window.processEvents()) {
            const auto currentTime = std::chrono::steady_clock::now();

            graphics::FrameContext frameContext;
            frameContext.time = std::chrono::duration<float>(currentTime - startTime).count();
            frameContext.deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
            frameContext.width = window.getWidth();
            frameContext.height = window.getHeight();

            lastFrameTime = currentTime;

            renderer->beginFrame();
            renderer->render(renderableScene, frameContext);
            renderer->endFrame();

            graphicsContext.swapBuffers();
        }
    }

} // namespace swarcs::accretion::app