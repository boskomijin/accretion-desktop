/**
 * @file stars.glsl
 * @brief Procedural background stars and dynamic constellation lines generation following SRP.
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
        float starSize = mix(0.008, 0.045, pow(sizeVariation, 9.0));

        float brightness = computeStarBrightness(cellId, t);
        float star = smoothstep(starSize, 0.0, dist) * brightness;
        float halo = smoothstep(starSize * 5.0, 0.0, dist) * brightness * 0.5;

        vec3 starColor = computeStarColor(cellId);
        starsTotal += starColor * (star + halo);
    }
    return starsTotal;
}

/**
 * @brief Renders rare, sparse, and organically distributed constellation star clusters.
 *
 * @param uv Warped coordinates for the starfield.
 * @param t Time variable for animation and drift.
 * @return vec3 RGB color contribution of the constellation clusters.
 */
vec3 renderConstellations(vec2 uv, float t) {
    vec2 driftUV = uv + vec2(t * 0.008, t * 0.005);

    vec2 gridScale = driftUV * 1.8;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    vec3 clusterTotal = vec3(0.0);

    float sectorHash = hash21(cellId * 37.13);
    if (sectorHash > 0.88) {
        vec2 clusterCenter = (vec2(hash21(cellId + 1.1), hash21(cellId + 2.2)) - 0.5) * 0.8;

        int starCount = 5 + int(hash21(cellId + 5.5) * 4.0);

        for (int i = 0; i < 9; i++) {
            if (i >= starCount) break;
            float idx = float(i);

            vec2 starOffset = vec2(
                hash21(cellId + vec2(idx, 3.3)) - 0.5,
                hash21(cellId + vec2(idx, 4.4)) - 0.5
            ) * 0.45;

            vec2 starPos = clusterCenter + starOffset;
            float dist = length(cellUv - starPos);

            float starSize = mix(0.002, 0.01, hash21(cellId + vec2(idx, 7.7)));

            float clusterPulse = sin(t * 0.5 + hash21(cellId + idx) * 6.28) * 0.25 + 0.75;
            float brightness = smoothstep(starSize, 0.0, dist) * clusterPulse;
            float halo = smoothstep(starSize * 3.5, 0.0, dist) * clusterPulse * 0.35;

            vec3 starColor = mix(vec3(0.5, 0.75, 1.0), vec3(1.0, 0.85, 0.6), hash21(cellId + vec2(idx, 8.8)));
            clusterTotal += starColor * (brightness + halo) * 1.2;
        }
    }
    return clusterTotal;
}