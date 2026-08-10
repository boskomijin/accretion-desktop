/**
* @file IRenderable.hpp
 * @brief Defines the polymorphic interface for renderable graphical objects.
 *
 * IRenderable establishes a pure virtual contract for any component or scene
 * that can submit itself to a renderer during the rendering loop.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

#pragma once

#include "swarcs/accretion/graphics/FrameContext.hpp"

namespace swarcs::accretion::graphics {

    class IRenderer; // Forward declaration

    /**
     * @brief Interface for renderable scene components that submit themselves to a renderer.
     *
     * Any class implementing this interface can be polymorphically submitted
     * to an active graphics renderer within the application's rendering pipeline.
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
         * @brief Submits the renderable geometry/scene to the specified renderer.
         *
         * Implementations must issue the necessary rendering commands or pass
         * geometry data to the active graphics renderer.
         *
         * @param renderer Reference to the active graphics renderer.
         * @param context Per-frame timing and resolution data.
         *
         * @author Bosko Mijin
         * @since 2026-08
         */
        virtual void submit(IRenderer& renderer, const FrameContext& context) const = 0;
    };

} // namespace swarcs::accretion::graphics