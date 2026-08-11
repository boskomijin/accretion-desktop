/**
 * @file quasars.glsl
 * @brief Distant high-energy quasars with chromatic halos and sparse, smooth radial falloff.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders distant high-energy quasars with distinct chromatic halos and seamless blending.
 *
 * @param warpedUV Warped coordinates.
 * @param t Time variable for animation and drift.
 * @return vec3 RGB color contribution of the quasars.
 */
vec3 renderQuasars(vec2 warpedUV, float t) {
    vec2 driftUV = warpedUV + vec2(t * 0.001, -t * 0.0015);

    vec3 quasarsTotal = vec3(0.0);

    vec2 gridScale = driftUV * 0.45;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float n = hash21(cellId * 119.7 + 45.2);
    if (n > 0.91) {
        vec2 quasarPos = vec2(hash21(cellId + 5.1) - 0.5, hash21(cellId + 6.2) - 0.5) * 0.4;
        float dist = length(cellUv - quasarPos);

        float slowFade = sin(t * 0.5 + hash21(cellId) * 6.28) * 0.4 + 0.6;
        float core = 0.0025 / (dist * dist + 0.0003);
        float diffractionRing = smoothstep(0.07, 0.0, abs(dist - 0.04)) * 0.25;

        float safeFalloff = max(0.0, 1.0 - dist * 2.5);
        safeFalloff = safeFalloff * safeFalloff;

        vec3 quasarColor = mix(vec3(1.0, 0.6, 0.2), vec3(0.4, 0.9, 1.0), hash21(cellId + 7.7));
        quasarsTotal += quasarColor * (core + diffractionRing) * slowFade * 1.0 * safeFalloff;
    }
    return quasarsTotal;
}