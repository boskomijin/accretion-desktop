/**
 * @file X11Window.hpp
 * @brief Manages X11 multi-monitor spanned desktop windows for wallpaper rendering.
 *
 * X11Window implements the IWindow interface using the Xlib library, handling
 * spanned screen geometry, wallpaper window hints, event polling, and native handle exposures.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

#include "swarcs/accretion/platform/IWindow.hpp"
#include <string_view>
#include <X11/Xlib.h>

namespace swarcs::accretion::platform {

/**
 * @brief RAII wrapper for an X11 window configured to span across dual monitors as a desktop background.
 *
 * Handles Xlib display connection, XRandR multi-monitor span calculation,
 * window creation with desktop window type atoms, and event processing.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class X11Window : public IWindow {
private:
    Display* display = nullptr; ///< Pointer to the Xlib display connection.
    Window window = 0;          ///< X11 window identifier handle.
    int width = 0;              ///< Spanned total window width in pixels.
    int height = 0;             ///< Spanned total window height in pixels.

public:
    /**
     * @brief Constructs an X11 window spanning across all available monitors and sets desktop hints.
     *
     * @param title Window title string.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    explicit X11Window(std::string_view title);

    /**
     * @brief Destroys the X11 window and closes the display connection.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    ~X11Window() override;

    // Delete copy operations to maintain unique ownership of X11 display and window resources
    X11Window(const X11Window&) = delete;
    X11Window& operator=(const X11Window&) = delete;

    /**
     * @brief Processes pending X11 event queue messages.
     *
     * @return true If the window should remain open.
     * @return false If a key press or close termination event is received.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    bool processEvents() override;

    /**
     * @brief Retrieves the spanned window width in pixels.
     *
     * @return int Total width across monitors.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    int getWidth() const override;

    /**
     * @brief Retrieves the spanned window height in pixels.
     *
     * @return int Total height across monitors.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    int getHeight() const override;

    /**
     * @brief Returns the native X11 display pointer as a generic void pointer.
     *
     * @return void* Native display handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void* getNativeDisplay() const override { return (void*)display; }

    /**
     * @brief Returns the native X11 window handle as a generic void pointer.
     *
     * @return void* Native window handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void* getNativeWindow() const override { return (void*)window; }

    /**
     * @brief Retrieves the raw Xlib Display pointer.
     *
     * @return Display* X display pointer.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    Display* getDisplay() const;

    /**
     * @brief Retrieves the raw X11 Window handle.
     *
     * @return Window X window ID.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    Window getWindow() const;
};

} // namespace swarcs::accretion::platform