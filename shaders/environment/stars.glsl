/**
 * @file stars.glsl
 * @brief Procedural background stars and energetic pulsars generation following SRP.
 *
 * @author Bosko Mijin
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
    float twinkle = sin(t * mix(0.5, 3.0, hash21(cellId + 8.1)) + hash21(cellId + 9.2) * 6.28) * 0.3 + 0.7;
    return mix(0.3, 1.5, hash21(cellId + 7.8)) * twinkle;
}

/**
 * @brief Computes spectral color temperature for a background star.
 *
 * @param cellId Cell grid identifier.
 * @return vec3 Calculated RGB star color.
 */
vec3 computeStarColor(vec2 cellId) {
    return mix(vec3(0.7, 0.8, 1.0), vec3(1.0, 0.9, 0.7), hash21(cellId + 9.0));
}

/**
 * @brief Renders procedural background stars with individual twinkling effects and slow drift.
 *
 * @param uv Warped coordinates for the starfield.
 * @param t Time variable for animation and drift.
 * @return vec3 RGB color contribution of the stars.
 */
vec3 renderBackgroundStars(vec2 uv, float t) {
    vec2 driftUV = uv + vec2(t * 0.008, t * 0.005);

    vec3 starsTotal = vec3(0.0);
    vec2 gridScale = driftUV * 12.0;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float clusterNoise = noise(cellId * 0.15);
    float threshold = mix(0.97, 0.85, clusterNoise);

    float n = hash21(cellId);
    if (n > threshold) {
        vec2 starPos = vec2(hash21(cellId + 1.2) - 0.5, hash21(cellId + 3.4) - 0.5) * 0.9;
        float dist = length(cellUv - starPos);
        float starSize = mix(0.008, 0.06, hash21(cellId + 5.6));

        float brightness = computeStarBrightness(cellId, t);
        float star = smoothstep(starSize, 0.0, dist) * brightness;

        vec3 starColor = computeStarColor(cellId);
        starsTotal += starColor * star;
    }
    return starsTotal;
}

/**
 * @brief Computes core flare intensity for a pulsating energetic pulsar.
 *
 * @param cellId Cell grid identifier.
 * @param dist Radial distance from cell center.
 * @param t Time variable for pulsing animation.
 * @return float Calculated pulsar core emission.
 */
float computePulsarCore(vec2 cellId, float dist, float t) {
    float pulseSpeed = mix(3.0, 6.0, hash21(cellId + 3.3));
    float pulse = pow(sin(t * pulseSpeed + hash21(cellId) * 10.0) * 0.5 + 0.5, 8.0);

    float core = (0.005 / (dist + 0.001)) * (pulse * 3.0 + 0.2);
    return clamp(core, 0.0, 5.0);
}

/**
 * @brief Renders rare pulsating energetic stars (pulsars) with intense flares and drift.
 *
 * @param warpedUV Warped coordinates.
 * @param t Time variable for pulsing animation and drift.
 * @return vec3 RGB color contribution of the pulsars.
 */
vec3 renderPulsars(vec2 warpedUV, float t) {
    vec2 driftUV = warpedUV + vec2(-t * 0.003, t * 0.004);

    vec3 pulsarsTotal = vec3(0.0);
    vec2 gridScale = driftUV * 0.6;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float n = hash21(cellId * 97.1);
    if (n > 0.99) {
        vec2 pulsarPos = vec2(hash21(cellId + 1.1) - 0.5, hash21(cellId + 2.2) - 0.5) * 0.5;
        float dist = length(cellUv - pulsarPos);

        float core = computePulsarCore(cellId, dist, t);
        vec3 pulsarColor = mix(vec3(0.4, 0.7, 1.0), vec3(1.0, 0.4, 0.8), hash21(cellId + 4.4));

        pulsarsTotal += pulsarColor * core;
    }
    return pulsarsTotal;
}