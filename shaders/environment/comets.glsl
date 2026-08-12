/**
 * @file comets.glsl
 * @brief Dynamic distant comets with fully randomized trajectories and natural fading tails.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders smoothly transitioning comets with unique angles and paths per cycle.
 *
 * @param uv Coordinates.
 * @param t Time variable for animation.
 * @return vec3 RGB color contribution of the comet.
 */
vec3 renderComets(vec2 uv, float t) {
    vec3 cometsTotal = vec3(0.0);

    // Continuous individual cycle per comet event
    float cometTime = t * 0.1;
    float cometId = floor(cometTime);
    float localT = fract(cometTime);

    // Fully randomize start position per cycle
    vec2 startPos = vec2(
        hash21(vec2(cometId, 1.1)) * 3.0 - 1.5,
        hash21(vec2(cometId, 2.2)) * 1.5 + 0.2
    );

    // Fully randomize movement angle and direction per cycle
    float angleVariation = hash21(vec2(cometId, 3.3)) * 0.8 - 0.4;
    vec2 direction = normalize(vec2(1.0, -0.4 + angleVariation));

    // Randomized speed and travel distance factor
    float speedFactor = 3.0 + hash21(vec2(cometId, 4.4)) * 2.5;
    vec2 headPos = startPos + direction * (localT * speedFactor);

    vec2 toPixel = uv - headPos;
    vec2 tailDir = -direction;
    float proj = dot(toPixel, tailDir);

    // Active tail and head projection window
    if (proj >= 0.0 && proj < 1.2) {
        vec2 perpVec = toPixel - tailDir * proj;
        float perpDist = length(perpVec);

        float tailWidth = 0.002 + proj * 0.015;
        float perpFade = smoothstep(tailWidth, 0.0, perpDist);
        float longitudinalFade = pow(1.0 - (proj / 1.2), 2.0);

        float core = perpFade * longitudinalFade;
        float headGlow = exp(-length(toPixel) * 350.0) * 3.0;

        // Smooth life fade-in and fade-out outside the main view
        float lifeFade = smoothstep(0.0, 0.15, localT) * smoothstep(1.0, 0.85, localT);

        vec3 cometColor = vec3(0.75, 0.88, 1.0);
        cometsTotal += cometColor * (core * 1.5 + headGlow) * lifeFade;
    }

    return cometsTotal;
}