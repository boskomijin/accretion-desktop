/**
 * @file EGLManager.hpp
 * @brief Manages EGL display, context, surface configuration, and rendering initialization.
 *
 * EGLManager implements the IGraphicsContext interface using EGL to bridge native windowing
 * systems with OpenGL ES rendering contexts.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

#include "swarcs/accretion/platform/IGraphicsContext.hpp"
#include <EGL/egl.h>

namespace swarcs::accretion::platform {

/**
 * @brief RAII wrapper and manager for EGL graphics context lifecycle.
 *
 * Handles EGL display connection, configuration matching, context creation,
 * window surface binding, and buffer presentation.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class EGLManager : public IGraphicsContext {
private:
    EGLDisplay eglDisplay = EGL_NO_DISPLAY; ///< Handle to the EGL display connection.
    EGLContext eglContext = EGL_NO_CONTEXT; ///< Handle to the EGL rendering context.
    EGLSurface eglSurface = EGL_NO_SURFACE; ///< Handle to the EGL drawing surface.
    EGLConfig eglConfig = nullptr;          ///< Selected EGL frame configuration settings.

public:
    /**
     * @brief Constructs the EGL manager using opaque native pointers to abstract platform types.
     *
     * @param nativeDisplay Pointer to the native display connection (e.g., X11 Display).
     * @param nativeWindow Pointer or handle to the native window surface.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    EGLManager(void* nativeDisplay, void* nativeWindow);

    /**
     * @brief Destroys the EGL manager and cleans up display, surface, and context resources.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    ~EGLManager() override;

    // Delete copy operations to ensure unique ownership of EGL hardware context resources
    EGLManager(const EGLManager&) = delete;
    EGLManager& operator=(const EGLManager&) = delete;

    /**
     * @brief Swaps the front and back buffers to present the rendered frame on screen.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void swapBuffers() override;

    /**
     * @brief Binds the EGL context and surface to the current rendering thread.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void makeCurrent();

    /**
     * @brief Retrieves the underlying EGL display handle.
     *
     * @return EGLDisplay Display handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    EGLDisplay getDisplay() const;

    /**
     * @brief Retrieves the underlying EGL rendering context handle.
     *
     * @return EGLContext Context handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    EGLContext getContext() const;

    /**
     * @brief Retrieves the underlying EGL window surface handle.
     *
     * @return EGLSurface Surface handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    EGLSurface getSurface() const;

    /**
     * @brief Retrieves the selected EGL configuration.
     *
     * @return EGLConfig Configuration pointer.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    EGLConfig getConfig() const;
};

} // namespace swarcs::accretion::platform