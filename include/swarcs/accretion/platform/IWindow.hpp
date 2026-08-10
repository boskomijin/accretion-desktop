/**
 * @file IWindow.hpp
 * @brief Defines the abstract interface for platform window management.
 *
 * IWindow establishes a polymorphic contract for window creation, event processing,
 * dimensions querying, and retrieval of native window handles across different windowing systems.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

namespace swarcs::accretion::platform {

/**
 * @brief Abstract interface representing a native window platform instance.
 *
 * Any class implementing this interface manages window events, screen dimensions,
 * and provides opaque platform-specific native handles for integration with graphics contexts.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */
class IWindow {
public:
    /**
     * @brief Virtual destructor ensuring proper cleanup of derived window objects.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    virtual ~IWindow() = default;

    /**
     * @brief Processes pending platform window events (e.g., input, close requests).
     *
     * @return true If the window remains open and execution should continue.
     * @return false If a close signal has been received.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    virtual bool processEvents() = 0;

    /**
     * @brief Retrieves the current width of the window in pixels.
     *
     * @return int Window width.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    [[nodiscard]] virtual int getWidth() const = 0;

    /**
     * @brief Retrieves the current height of the window in pixels.
     *
     * @return int Window height.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    [[nodiscard]] virtual int getHeight() const = 0;

    /**
     * @brief Retrieves an opaque pointer to the native display connection.
     *
     * @return void* Native display handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    [[nodiscard]] virtual void* getNativeDisplay() const = 0;

    /**
     * @brief Retrieves an opaque pointer or handle to the native window surface.
     *
     * @return void* Native window handle.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    [[nodiscard]] virtual void* getNativeWindow() const = 0;
};

} // namespace swarcs::accretion::platform