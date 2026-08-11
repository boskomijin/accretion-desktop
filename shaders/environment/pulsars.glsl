/**
 * @file pulsars.glsl
 * @brief Rare pulsating energetic stars (pulsars) with sparse distribution and smooth radial falloff.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Computes core flare intensity for a pulsating energetic pulsar.
 *
 * @param cellId Cell grid identifier.
 * @param dist Radial distance from cell center.
 * @param t Time variable for pulsing animation.
 * @return float Calculated pulsar core emission.
 */
float computePulsarCore(vec2 cellId, float dist, float t) {
    float pulseSpeed = mix(2.5, 7.0, hash21(cellId + 3.3));
    float pulse = pow(sin(t * pulseSpeed + hash21(cellId) * 10.0) * 0.5 + 0.5, 6.0);

    float core = (0.006 / (dist * dist + 0.0003)) * (pulse * 3.5 + 0.2);
    return clamp(core, 0.0, 4.0);
}

/**
 * @brief Renders pulsating energetic stars (pulsars) with seamless radial containment.
 *
 * @param warpedUV Warped coordinates.
 * @param t Time variable for pulsing animation and drift.
 * @return vec3 RGB color contribution of the pulsars.
 */
vec3 renderPulsars(vec2 warpedUV, float t) {
    vec2 driftUV = warpedUV + vec2(-t * 0.002, t * 0.003);

    vec3 pulsarsTotal = vec3(0.0);

    vec2 gridScale = driftUV * 0.55;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float n = hash21(cellId * 73.3 + 12.4);
    if (n > 0.90) {
        vec2 pulsarPos = vec2(hash21(cellId + 1.1) - 0.5, hash21(cellId + 2.2) - 0.5) * 0.4;
        float dist = length(cellUv - pulsarPos);

        float core = computePulsarCore(cellId, dist, t);

        float safeFalloff = max(0.0, 1.0 - dist * 2.5);
        safeFalloff = safeFalloff * safeFalloff;

        vec3 pulsarColor = mix(vec3(0.3, 0.6, 1.0), vec3(1.0, 0.3, 0.7), hash21(cellId + 4.4));

        pulsarsTotal += pulsarColor * core * safeFalloff;
    }
    return pulsarsTotal;
}