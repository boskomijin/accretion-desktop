/**
 * @file WaylandSurface.cpp
 * @brief Implementation of WaylandSurface for Wayland compositor and EGL integration.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/platform/WaylandSurface.hpp"
#include <iostream>
#include <stdexcept>

namespace swarcs::accretion::platform {

/**
 * @brief Constructs the Wayland surface via compositor, creates the native EGL window, and initializes the surface.
 *
 * @param display Wayland display connection.
 * @param compositor Wayland compositor interface.
 * @param eglDisplay EGL display handle.
 * @param config EGL config.
 * @param width Width in pixels.
 * @param height Height in pixels.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
WaylandSurface::WaylandSurface(struct wl_display* display, struct wl_compositor* compositor, EGLDisplay eglDisplay, EGLConfig config, int width, int height) {
    // Create the base Wayland surface through the compositor
    surface = wl_compositor_create_surface(compositor);
    if (!surface) {
        throw std::runtime_error("Failed to create wl_surface!");
    }

    // Create the EGL native window wrapper for Wayland
    eglWindow = wl_egl_window_create(surface, width, height);
    if (!eglWindow) {
        throw std::runtime_error("Failed to create wl_egl_window!");
    }

    // Create the EGL window surface from the Wayland EGL window
    eglSurface = eglCreateWindowSurface(eglDisplay, config, (EGLNativeWindowType)eglWindow, nullptr);
    if (eglSurface == EGL_NO_SURFACE) {
        throw std::runtime_error("Failed to create EGL surface!");
    }

    std::cout << "Wayland surface and EGL window successfully linked.\n";
}

/**
 * @brief Destroys the Wayland surface and frees EGL window objects.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
WaylandSurface::~WaylandSurface() {
    if (eglWindow) {
        wl_egl_window_destroy(eglWindow);
    }
    if (surface) {
        wl_surface_destroy(surface);
    }
}

/**
 * @brief Returns the EGL surface handle.
 *
 * @return EGLSurface EGL surface.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLSurface WaylandSurface::getEGLSurface() const {
    return eglSurface;
}

/**
 * @brief Returns the raw Wayland surface pointer.
 *
 * @return struct wl_surface* Wayland surface.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
struct wl_surface* WaylandSurface::getSurface() const {
    return surface;
}

} // namespace swarcs::accretion::platform