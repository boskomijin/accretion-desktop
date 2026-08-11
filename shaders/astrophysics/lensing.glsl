/**
 * @file lensing.glsl
 * @brief Gravitational lensing approximation following SRP.
 *
 * Provides a simple Schwarzschild-like bending of rays near the event horizon.
 * This module is designed to be composited in fragment.glsl for background warping.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Checks if the given position falls within the black hole event horizon.
 *
 * @param r Radial distance from the black hole center.
 * @param eventHorizon Radius of the event horizon.
 * @return bool True if position is inside or on the horizon.
 */
bool isInsideHorizon(float r, float eventHorizon) {
    return r < eventHorizon;
}

/**
 * @brief Computes the localized gravitational lensing distortion factor.
 *
 * @param distFromHorizon Distance from the event horizon boundary.
 * @param curvature Strength factor for bending.
 * @return float Calculated distortion magnitude.
 */
float computeLensingDistortion(float distFromHorizon, float curvature) {
    return curvature * exp(-distFromHorizon * 8.0) / (distFromHorizon + 0.02);
}

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
    if (isInsideHorizon(r, eventHorizon)) {
        return pos;
    }

    vec2 dir = normalize(pos);
    float distFromHorizon = r - eventHorizon;
    float lensingFactor = computeLensingDistortion(distFromHorizon, curvature);

    return pos + dir * lensingFactor * 0.03;
}