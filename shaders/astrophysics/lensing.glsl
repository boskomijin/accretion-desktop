/**
 * @file lensing.glsl
 * @brief Gravitational lensing approximation for background light around a black hole.
 *
 * Provides a simple Schwarzschild-like bending of rays near the event horizon.
 * This module is designed to be composited in fragment.glsl for background warping.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Applies gravitational lensing to a position vector.
 *
 * @param pos Position relative to black hole center.
 * @param eventHorizon Radius of the black hole event horizon.
 * @param curvature Strength factor for bending.
 * @return vec2 Warped position for background sampling.
 */
vec2 applyGravitationalLensing(vec2 pos, float eventHorizon, float curvature) {
    float r = length(pos);
    if (r < eventHorizon) {
        return pos; // Inside horizon, no background
    }
    vec2 dir = normalize(pos);

    // Lensing deluje isključivo u uzkoj zoni blizu horizonta, a ne razliva se na daleko
    float distFromHorizon = r - eventHorizon;
    float lensingFactor = curvature * exp(-distFromHorizon * 8.0) / (distFromHorizon + 0.02);

    return pos + dir * lensingFactor * 0.03;
}
