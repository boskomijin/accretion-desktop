/**
* @file IRenderable.hpp
 * @brief Defines the polymorphic interface for renderable graphical objects.
 *
 * IRenderable establishes a pure virtual contract for any component or scene
 * that can be drawn onto the graphics canvas during the rendering loop.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

namespace swarcs::accretion::graphics {

    /**
     * @brief Abstract interface representing a renderable entity.
     *
     * Any class implementing this interface can be polymorphically drawn
     * within the application's rendering pipeline.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    class IRenderable {
    public:
        /**
         * @brief Virtual destructor ensuring proper cleanup of derived renderable objects.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        virtual ~IRenderable() = default;

        /**
         * @brief Pure virtual method for executing draw operations.
         *
         * Implementations must issue the necessary rendering commands to draw
         * the object onto the active graphics context.
         *
         * @author Bosko Mijin
         * @since 2026-02
         */
        virtual void draw() const = 0;
    };

} // namespace swarcs::accretion::graphics