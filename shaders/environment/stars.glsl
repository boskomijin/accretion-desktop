/**
 * @file stars.glsl
 * @brief Procedural background stars and energetic pulsars generation.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders procedural background stars with individual twinkling effects.
 *
 * @param uv Warped coordinates for the starfield.
 * @param t Time variable for animation.
 * @return vec3 RGB color contribution of the stars.
 */
vec3 renderBackgroundStars(vec2 uv, float t) {
    vec3 starsTotal = vec3(0.0);
    vec2 gridScale = uv * 12.0;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float clusterNoise = noise(cellId * 0.15);
    float threshold = mix(0.97, 0.85, clusterNoise);

    float n = hash21(cellId);
    if (n > threshold) {
        vec2 starPos = vec2(hash21(cellId + 1.2) - 0.5, hash21(cellId + 3.4) - 0.5) * 0.9;
        float dist = length(cellUv - starPos);
        float starSize = mix(0.008, 0.06, hash21(cellId + 5.6));
        float twinkle = sin(t * mix(0.5, 3.0, hash21(cellId + 8.1)) + hash21(cellId + 9.2) * 6.28) * 0.3 + 0.7;
        float brightness = mix(0.3, 1.5, hash21(cellId + 7.8)) * twinkle;

        float star = smoothstep(starSize, 0.0, dist) * brightness;
        vec3 starColor = mix(vec3(0.7, 0.8, 1.0), vec3(1.0, 0.9, 0.7), hash21(cellId + 9.0));
        starsTotal += starColor * star;
    }
    return starsTotal;
}

/**
 * @brief Renders rare pulsating energetic stars (pulsars) with intense flares.
 *
 * @param warpedUV Warped coordinates.
 * @param t Time variable for pulsing animation.
 * @return vec3 RGB color contribution of the pulsars.
 */
vec3 renderPulsars(vec2 warpedUV, float t) {
    vec3 pulsarsTotal = vec3(0.0);
    vec2 gridScale = warpedUV * 0.6;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float n = hash21(cellId * 97.1);
    if (n > 0.99) {
        vec2 pulsarPos = vec2(hash21(cellId + 1.1) - 0.5, hash21(cellId + 2.2) - 0.5) * 0.5;
        float dist = length(cellUv - pulsarPos);

        float pulseSpeed = mix(3.0, 6.0, hash21(cellId + 3.3));
        float pulse = pow(sin(t * pulseSpeed + hash21(cellId) * 10.0) * 0.5 + 0.5, 8.0);

        float core = (0.005 / (dist + 0.001)) * (pulse * 3.0 + 0.2);
        core = clamp(core, 0.0, 5.0);

        vec3 pulsarColor = mix(vec3(0.4, 0.7, 1.0), vec3(1.0, 0.4, 0.8), hash21(cellId + 4.4));
        pulsarsTotal += pulsarColor * core;
    }
    return pulsarsTotal;
}