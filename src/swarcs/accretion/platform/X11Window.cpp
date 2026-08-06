/**
 * @file X11Window.cpp
 * @brief Implementation of X11Window for multi-monitor desktop wallpaper window management.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#include "swarcs/accretion/platform/X11Window.hpp"
#include <iostream>
#include <stdexcept>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

namespace swarcs::accretion::platform {

/**
 * @brief Constructs the X11Window, queries XRandR multi-monitor layouts, and sets wallpaper window properties.
 *
 * @param title Window title text.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
X11Window::X11Window(std::string_view title) {
    // Open connection to the X server display
    display = XOpenDisplay(nullptr);
    if (!display) {
        throw std::runtime_error("Failed to open X display!");
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    int total_w = 0;
    int total_h = 0;

    // Use XRandR extension to calculate combined bounding box across all active physical monitors
    int nmonitors = 0;
    XRRMonitorInfo* monitors = XRRGetMonitors(display, root, True, &nmonitors);
    if (monitors && nmonitors > 0) {
        for (int i = 0; i < nmonitors; i++) {
            int right_edge = monitors[i].x + monitors[i].width;
            int bottom_edge = monitors[i].y + monitors[i].height;
            if (right_edge > total_w) total_w = right_edge;
            if (bottom_edge > total_h) total_h = bottom_edge;
        }
        XFree(monitors);
    }

    // Fallback to default screen dimensions if XRandR returns invalid dimensions
    if (total_w <= 0 || total_h <= 0) {
        total_w = DisplayWidth(display, screen);
        total_h = DisplayHeight(display, screen);
    }

    width = total_w;
    height = total_h;

    std::cout << "Creating spanned wallpaper canvas across monitors: " << width << "x" << height << "\n";

    // Create the X11 window spanning the combined screen resolution
    window = XCreateSimpleWindow(display, root, 0, 0, width, height, 0,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // Set EWMH window type property to _NET_WM_WINDOW_TYPE_DESKTOP so it behaves as background wallpaper
    Atom net_wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    Atom net_wm_window_type_desktop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    XChangeProperty(display, window, net_wm_window_type, XA_ATOM, 32, PropModeReplace,
                    (unsigned char*)&net_wm_window_type_desktop, 1);

    // Register interest in key press and structure notification events
    XSelectInput(display, window, KeyPressMask | StructureNotifyMask);

    // Map window to screen, assign title, and flush request buffer
    XMapWindow(display, window);
    XStoreName(display, window, title.data());
    XFlush(display);

    std::cout << "Window successfully configured and set as desktop background.\n";
}

/**
 * @brief Destroys the X11 window and closes the X display connection.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
X11Window::~X11Window() {
    if (display && window) {
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }
}

/**
 * @brief Processes pending X11 events from the server queue.
 *
 * @return true If execution should continue.
 * @return false If a key press event is triggered to close the window.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
bool X11Window::processEvents() {
    while (XPending(display) > 0) {
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Returns the Xlib display pointer.
 *
 * @return Display* Display pointer.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
Display* X11Window::getDisplay() const { return display; }

/**
 * @brief Returns the X11 window ID.
 *
 * @return Window Window identifier.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
Window X11Window::getWindow() const { return window; }

/**
 * @brief Returns the window width in pixels.
 *
 * @return int Width.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
int X11Window::getWidth() const { return width; }

/**
 * @brief Returns the window height in pixels.
 *
 * @return int Height.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
int X11Window::getHeight() const { return height; }

} // namespace swarcs::accretion::platform