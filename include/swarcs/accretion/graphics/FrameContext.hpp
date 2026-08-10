#pragma once

namespace swarcs::accretion::graphics {

    /**
     * @brief Encapsulates per-frame data required for rendering scenes and animations.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */
    struct FrameContext {
        float time = 0.0f;       ///< Monotonic elapsed time in seconds since application start.
        float deltaTime = 0.0f;  ///< Time elapsed since the previous frame.
        int width = 0;           ///< Current window/viewport width in pixels.
        int height = 0;          ///< Current window/viewport height in pixels.
    };

} // namespace swarcs::accretion::graphics