/**
 * @file math.glsl
 * @brief Common mathematical helper functions for graphics and space transformations.
 *
 * Provides utility functions for 2D plane rotations, pseudo-random noise generation,
 * and vector manipulation.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Rotates a 2D point around the origin by a given angle.
 *
 * @param pt 2D input position vector.
 * @param angle Rotation angle in radians.
 * @return vec2 Rotated 2D vector.
 */
vec2 rotate2D(vec2 pt, float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat2(c, -s, s, c) * pt;
}

/**
 * @brief Generates a pseudo-random floating-point value based on a 2D coordinate.
 *
 * Utilizes a trigonometric sine combination for deterministic noise generation.
 *
 * @param st 2D coordinate input (e.g., UV coordinates or screen position).
 * @return float Pseudo-random value in the range [0.0, 1.0].
 */
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}