/**
* @file IGraphicsContext.hpp
 * @brief Defines the abstract interface for graphics rendering contexts.
 *
 * IGraphicsContext establishes a polymorphic contract for managing graphics
 * context operations such as buffer swapping across different windowing/rendering backends.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

namespace swarcs::accretion::platform {

    /**
     * @brief Abstract interface representing a graphics rendering context.
     *
     * Any class implementing this interface handles window-system-agnostic
     * rendering context operations like frame presentation and buffer swapping.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class IGraphicsContext {
    public:
        /**
         * @brief Virtual destructor ensuring proper cleanup of derived graphics context objects.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        virtual ~IGraphicsContext() = default;

        /**
         * @brief Pure virtual method for swapping rendering buffers.
         *
         * Implementations must present the rendered back buffer onto the display surface.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        virtual void swapBuffers() = 0;
    };

} // namespace swarcs::accretion::platform