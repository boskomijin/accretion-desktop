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
#include <algorithm>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

namespace swarcs::accretion::platform {

/**
 * @brief Queries XRandR extension to calculate the bounding box across all active physical monitors.
 *
 * @return ScreenGeometry Calculated bounding box position and total resolution.
 */
X11Window::ScreenGeometry X11Window::querySpannedGeometry() const {
    const int screen = DefaultScreen(display);
    const Window root = RootWindow(display, screen);

    int nmonitors = 0;
    if (XRRMonitorInfo* monitors = XRRGetMonitors(display, root, True, &nmonitors); monitors && nmonitors > 0) {
        int minX = 0;
        int minY = 0;
        int maxX = 0;
        int maxY = 0;
        bool first = true;

        for (int i = 0; i < nmonitors; i++) {
            int curX = monitors[i].x;
            int curY = monitors[i].y;
            const int curW = monitors[i].width;
            const int curH = monitors[i].height;

            if (first) {
                minX = curX;
                minY = curY;
                maxX = curX + curW;
                maxY = curY + curH;
                first = false;
            } else {
                minX = std::min(minX, curX);
                minY = std::min(minY, curY);
                maxX = std::max(maxX, curX + curW);
                maxY = std::max(maxY, curY + curH);
            }
        }
        XFree(monitors);

        return {minX, minY, maxX - minX, maxY - minY};
    }

    // Fallback to default screen dimensions if XRandR returns invalid dimensions
    return {0, 0, DisplayWidth(display, screen), DisplayHeight(display, screen)};
}

/**
 * @brief Sets EWMH window type property to _NET_WM_WINDOW_TYPE_DESKTOP.
 */
void X11Window::setDesktopWindowHints() const {
    const Atom net_wm_window_type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    Atom net_wm_window_type_desktop = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
    XChangeProperty(display, window, net_wm_window_type, XA_ATOM, 32, PropModeReplace,
                    reinterpret_cast<unsigned char *>(&net_wm_window_type_desktop), 1);
}

/**
 * @brief Constructs the X11Window, queries XRandR multi-monitor layouts, and sets wallpaper window properties.
 *
 * @param title Window title text.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
X11Window::X11Window(const std::string_view title) {
    display = XOpenDisplay(nullptr);
    if (!display) {
        throw std::runtime_error("Failed to open X display!");
    }

    try {
        const int screen = DefaultScreen(display);
        const Window root = RootWindow(display, screen);

        const auto geometry = querySpannedGeometry();
        width = geometry.width;
        height = geometry.height;

        std::cout << "Creating spanned wallpaper canvas at (" << geometry.x << ", " << geometry.y
                  << ") with dimensions: " << width << "x" << height << "\n";

        window = XCreateSimpleWindow(display, root, geometry.x, geometry.y, width, height, 0,
                                     BlackPixel(display, screen), WhitePixel(display, screen));

        setDesktopWindowHints();

        XSelectInput(display, window, KeyPressMask | StructureNotifyMask);
        XMapWindow(display, window);
        XStoreName(display, window, title.data());
        XFlush(display);

        std::cout << "Window successfully configured and set as desktop background.\n";
    } catch (...) {
        if (display) {
            if (window) {
                XDestroyWindow(display, window);
            }
            XCloseDisplay(display);
            display = nullptr;
            window = 0;
        }
        throw;
    }
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