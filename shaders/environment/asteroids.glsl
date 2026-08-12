/**
 * @file asteroids.glsl
 * @brief Discrete, organically shaped irregular asteroid clusters in the deep background.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders isolated asteroid clusters with randomized irregular cloud-like shapes.
 *
 * @param uv Coordinates.
 * @param t Time variable for slow drift.
 * @return vec3 RGB color contribution of the asteroid clusters.
 */
vec3 renderAsteroids(vec2 uv, float t) {
    // Very slow parallax drift in the deep background
    vec2 driftUV = uv + vec2(t * 0.0004, -t * 0.0002);

    vec3 asteroidsTotal = vec3(0.0);

    // Fixed centers for isolated asteroid islands
    vec2 centers[3];
    centers[0] = vec2(-0.8,  0.4);
    centers[1] = vec2( 0.6, -0.5);
    centers[2] = vec2( 0.2,  0.7);

    for(int i = 0; i < 3; i++) {
        // Gentle floating movement per island
        vec2 center = centers[i] + vec2(sin(t * 0.0002 + float(i)) * 0.08, cos(t * 0.00015 * float(i + 1)) * 0.08);

        vec2 toCenter = driftUV - center;
        float dist = length(toCenter);
        float angle = atan(toCenter.y, toCenter.x);

        float seed = float(i + 1) * 37.15;

        // Non-symmetric, organic blob radius using lower frequencies (1.0 and 2.0) to avoid clover/butterfly shapes
        float irregularRadius = 0.25 +
            sin(angle * 1.0 + seed) * 0.08 +
            cos(angle * 2.0 + seed * 1.5) * 0.06 +
            sin(angle * 4.0 - seed) * 0.03;

        // Soft edge fade
        float islandMask = smoothstep(irregularRadius, irregularRadius - 0.1, dist);

        // Finer, more natural rock texture inside the cluster
        float grain = hash21(uv * 80.0 + float(i) * 12.3);
        float discreteRocks = smoothstep(0.4, 0.65, grain) * islandMask;

        vec3 rockColor = vec3(0.32, 0.35, 0.4);
        asteroidsTotal += rockColor * discreteRocks * 0.22;
    }

    return asteroidsTotal;
}