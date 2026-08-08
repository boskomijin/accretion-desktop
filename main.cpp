/**
 * @file main.cpp
 * @brief Application entry point and composition root for AccretionDesktop.
 *
 * Initializes platform infrastructure, sets up the graphics context and rendering resources,
 * wires up dependencies using dependency injection, and launches the main execution loop.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/app/AccretionApp.hpp"
#include "swarcs/accretion/platform/X11Window.hpp"
#include "swarcs/accretion/platform/EGLManager.hpp"
#include "swarcs/accretion/graphics/FullScreenQuad.hpp"
#include <iostream>
#include <memory>
#include <exception>

/**
 * @brief Main entry point of the application.
 *
 * Sets up the composition root, handles initialization errors gracefully,
 * and controls the overall lifecycle of the Accretion wallpaper application.
 *
 * @return int Exit code (0 on success, 1 on fatal error).
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
int main() {
    try {
        // 1. Initialize platform infrastructure (X11 Spanned Window spanning both displays)
        const auto window = std::make_unique<swarcs::accretion::platform::X11Window>("Jarvis Wallpaper Spanned");

        // 2. Initialize graphics context (EGL context automatically binds upon construction)
        const auto eglContext = std::make_unique<swarcs::accretion::platform::EGLManager>(
            window->getNativeDisplay(),
            window->getNativeWindow()
        );

        std::cout << "Graphics context successfully initialized.\n";

        // 3. Create renderable graphics resources (Full-screen quad for shader execution)
        const auto scene = std::make_unique<swarcs::accretion::graphics::FullScreenQuad>();

        // 4. Perform Dependency Injection (DI) via references (*window, *eglContext, *scene)
        swarcs::accretion::app::AccretionApp app(*window, *eglContext, *scene);

        // 5. Start the core application loop
        app.run();
    } catch (const std::exception& e) {
        // Catch and report any fatal exceptions during execution
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}