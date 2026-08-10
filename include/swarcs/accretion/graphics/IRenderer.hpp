#pragma once

#include "swarcs/accretion/graphics/IRenderable.hpp"
#include "swarcs/accretion/graphics/FrameContext.hpp"

namespace swarcs::accretion::graphics {

    /**
     * @brief Interface for rendering graphics backends (API agnostic).
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    class IRenderer {
    public:
        virtual ~IRenderer() = default;

        virtual void beginFrame() = 0;
        virtual void render(const IRenderable& scene, const FrameContext& frameContext) = 0;
        virtual void endFrame() = 0;
    };

} // namespace swarcs::accretion::graphics