/**
 * @file WaylandSurface.hpp
 * @brief Manages Wayland client surfaces, EGL native windows, and rendering surfaces.
 *
 * WaylandSurface encapsulates Wayland compositor surface creation and binds it
 * to EGL window surfaces for hardware-accelerated rendering on Wayland compositors.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>

namespace swarcs::accretion::platform {

/**
 * @brief RAII wrapper for Wayland surface and EGL native window integration.
 *
 * Manages the lifecycle of a `wl_surface`, `wl_egl_window`, and the corresponding
 * EGL drawing surface for Wayland display servers.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class WaylandSurface {
private:
    struct wl_surface* surface = nullptr;       ///< Pointer to the Wayland client surface.
    struct wl_egl_window* eglWindow = nullptr;  ///< Pointer to the Wayland EGL native window wrapper.
    EGLSurface eglSurface = EGL_NO_SURFACE;     ///< Handle to the EGL rendering surface.

public:
    /**
     * @brief Constructs a Wayland surface and binds it to an EGL display and configuration.
     *
     * @param display Pointer to the Wayland display connection.
     * @param compositor Pointer to the Wayland compositor global interface.
     * @param eglDisplay EGL display handle.
     * @param config EGL configuration settings.
     * @param width Initial surface width in pixels.
     * @param height Initial surface height in pixels.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    WaylandSurface(struct wl_display* display, struct wl_compositor* compositor, EGLDisplay eglDisplay, EGLConfig config, int width, int height);

    /**
     * @brief Destroys the Wayland surface, EGL window, and releases resources.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    ~WaylandSurface();

    // Delete copy operations to ensure unique ownership of Wayland surface resources
    WaylandSurface(const WaylandSurface&) = delete;
    WaylandSurface& operator=(const WaylandSurface&) = delete;

    /**
     * @brief Retrieves the underlying EGL rendering surface handle.
     *
     * @return EGLSurface EGL surface handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    EGLSurface getEGLSurface() const;

    /**
     * @brief Retrieves the raw Wayland surface pointer.
     *
     * @return struct wl_surface* Wayland surface pointer.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    struct wl_surface* getSurface() const;
};

} // namespace swarcs::accretion::platform