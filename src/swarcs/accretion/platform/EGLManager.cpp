/**
 * @file EGLManager.cpp
 * @brief Implementation of EGLManager for EGL and OpenGL ES context setup.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/platform/EGLManager.hpp"
#include <X11/Xlib.h>
#include <iostream>
#include <stdexcept>

namespace swarcs::accretion::platform {

/**
 * @brief Constructs the EGLManager by initializing display connection, attributes, context, and surface.
 *
 * @param nativeDisplay Native display pointer.
 * @param nativeWindow Native window handle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLManager::EGLManager(void* nativeDisplay, void* nativeWindow) {
    auto* display = static_cast<Display*>(nativeDisplay);
    auto window = reinterpret_cast<Window>(nativeWindow);

    // Obtain EGL display connection from native display
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)display);
    if (eglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("Failed to retrieve EGL display!");
    }

    // Initialize EGL library
    if (!eglInitialize(eglDisplay, nullptr, nullptr)) {
        throw std::runtime_error("Failed to initialize EGL!");
    }

    // Define desired framebuffer attributes for OpenGL ES 2.0 rendering
    constexpr EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_NONE
    };

    EGLint numConfigs;
    if (!eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs) || numConfigs == 0) {
        throw std::runtime_error("No matching EGL configurations found!");
    }

    // Specify OpenGL ES version 3 context creation attributes
    constexpr EGLint ctxAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    // Create rendering context
    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttribs);
    if (eglContext == EGL_NO_CONTEXT) {
        throw std::runtime_error("Failed to create EGL context!");
    }

    // Create window surface matching the native window
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType)window, nullptr);
    if (eglSurface == EGL_NO_SURFACE) {
        throw std::runtime_error("Failed to create EGL window surface!");
    }

    // Activate the created context and surface
    makeCurrent();
    std::cout << "EGL successfully initialized and bound.\n";
}

/**
 * @brief Destroys the EGL manager, releasing surfaces, contexts, and terminating display connections.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLManager::~EGLManager() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglSurface != EGL_NO_SURFACE) eglDestroySurface(eglDisplay, eglSurface);
        if (eglContext != EGL_NO_CONTEXT) eglDestroyContext(eglDisplay, eglContext);
        eglTerminate(eglDisplay);
    }
}

/**
 * @brief Swaps display back and front buffers to present rendered frames.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void EGLManager::swapBuffers() {
    eglSwapBuffers(eglDisplay, eglSurface);
}

/**
 * @brief Makes the EGL context current on the calling thread.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
void EGLManager::makeCurrent() const {
    if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
        throw std::runtime_error("Failed to make EGL context current!");
    }
}

/**
 * @brief Returns the EGL display handle.
 *
 * @return EGLDisplay Display handle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLDisplay EGLManager::getDisplay() const { return eglDisplay; }

/**
 * @brief Returns the EGL context handle.
 *
 * @return EGLContext Context handle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLContext EGLManager::getContext() const { return eglContext; }

/**
 * @brief Returns the EGL surface handle.
 *
 * @return EGLSurface Surface handle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLSurface EGLManager::getSurface() const { return eglSurface; }

/**
 * @brief Returns the EGL configuration.
 *
 * @return EGLConfig Configuration handle.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
EGLConfig EGLManager::getConfig() const { return eglConfig; }

} // namespace swarcs::accretion::platform