/**
 * @file stars.glsl
 * @brief Procedural background stars generation with rich size and luminescence variation following SRP.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Computes twinkling and brightness intensity for a background star.
 *
 * @param cellId Cell grid identifier.
 * @param t Time variable for animation.
 * @return float Calculated star brightness factor.
 */
float computeStarBrightness(vec2 cellId, float t) {
    float twinkle = sin(t * mix(0.4, 4.0, hash21(cellId + 8.1)) + hash21(cellId + 9.2) * 6.28) * 0.35 + 0.65;
    return mix(0.2, 2.5, hash21(cellId + 7.8)) * twinkle;
}

/**
 * @brief Computes spectral color temperature and class variation for a background star.
 *
 * @param cellId Cell grid identifier.
 * @return vec3 Calculated RGB star color.
 */
vec3 computeStarColor(vec2 cellId) {
    float type = hash21(cellId + 9.0);
    vec3 col = mix(vec3(0.6, 0.75, 1.0), vec3(1.0, 0.9, 0.7), type);
    if (type > 0.9) {
        col = vec3(0.5, 0.8, 1.0); // Blue giant
    } else if (type < 0.1) {
        col = vec3(1.0, 0.5, 0.4); // Red giant
    }
    return col;
}

/**
 * @brief Renders procedural background stars with wide variations in size and luminescence.
 *
 * @param uv Warped coordinates for the starfield.
 * @param t Time variable for animation and drift.
 * @return vec3 RGB color contribution of the stars.
 */
vec3 renderBackgroundStars(vec2 uv, float t) {
    vec2 driftUV = uv + vec2(t * 0.008, t * 0.005);

    vec3 starsTotal = vec3(0.0);
    vec2 gridScale = driftUV * 16.0;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float clusterNoise = noise(cellId * 0.15);
    float threshold = mix(0.96, 0.80, clusterNoise);

    float n = hash21(cellId);
    if (n > threshold) {
        vec2 starPos = vec2(hash21(cellId + 1.2) - 0.5, hash21(cellId + 3.4) - 0.5) * 0.8;
        float dist = length(cellUv - starPos);

        float sizeVariation = hash21(cellId + 5.6);
        float starSize = mix(0.004, 0.045, pow(sizeVariation, 2.0));

        float brightness = computeStarBrightness(cellId, t);
        float star = smoothstep(starSize, 0.0, dist) * brightness;
        float halo = smoothstep(starSize * 3.0, 0.0, dist) * brightness * 0.25;

        vec3 starColor = computeStarColor(cellId);
        starsTotal += starColor * (star + halo);
    }
    return starsTotal;
}