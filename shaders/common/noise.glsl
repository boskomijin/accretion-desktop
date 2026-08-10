/**
 * @file noise.glsl
 * @brief Common mathematical utilities, hash functions, Perlin noise, and FBM algorithms.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Generates a 2D rotation matrix for a given angle in radians.
 */
mat2 rotate(float angle) {
    float s = sin(angle), c = cos(angle);
    return mat2(c, -s, s, c);
}

/**
 * @brief Pseudo-random hash function from 2D coordinate to float [0, 1].
 */
float hash21(vec2 p) {
    p = fract(p * vec2(234.34, 435.345));
    p += dot(p, p + 34.23);
    return fract(p.x * p.y);
}

/**
 * @brief 2D value noise function.
 */
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(hash21(i + vec2(0.0, 0.0)), hash21(i + vec2(1.0, 0.0)), u.x),
               mix(hash21(i + vec2(0.0, 1.0)), hash21(i + vec2(1.0, 1.0)), u.x), u.y);
}

/**
 * @brief Fractional Brownian Motion (FBM) combining multiple octaves of noise.
 */
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise(p * frequency);
        frequency *= 2.1;
        amplitude *= 0.48;
    }
    return value;
}