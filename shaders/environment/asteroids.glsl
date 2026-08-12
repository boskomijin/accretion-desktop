/**
 * @file asteroids.glsl
 * @brief Renders procedural organic asteroid dust clouds distributed across fixed quadrants.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders procedural asteroid dust clouds with organic noise and quadrant separation.
 *
 * @param uv Normalized screen or background coordinates.
 * @param t Time variable governing slow drift movement.
 * @return vec3 Computed RGB color contribution of the asteroid dust clouds.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */
vec3 renderAsteroids(vec2 uv, float t) {
    vec3 dustTotal = vec3(0.0);

    // Define 4 fixed quadrants to distribute dust clouds evenly across screen corners
    vec2 offsets[4] = vec2[4](
        vec2(-1.8,  0.8), // Top-left
        vec2( 1.5,  0.7), // Top-right
        vec2(-1.4, -0.7), // Bottom-left
        vec2( 1.6, -0.6)  // Bottom-right
    );

    for(int i = 0; i < 4; i++) {
        float fi = float(i);

        // Fetch predefined quadrant offsets and introduce subtle micro-randomness
        vec2 basePos = offsets[i] + vec2(
            (hash21(vec2(fi * 11.1, 22.2)) - 0.5) * 0.6,
            (hash21(vec2(fi * 33.3, 44.4)) - 0.5) * 0.4
        );

        float depth = hash21(vec2(fi * 15.45, 83.12)) * 1.5 + 0.6;
        vec2 drift = vec2(t * 0.00003, -t * 0.000015) / depth;
        vec2 p = uv - (basePos + drift);

        float scale = hash21(vec2(fi * 52.31, 38.93)) * 1.1 + 0.9;
        p *= scale * depth;

        // Clean organic multi-octave noise accumulation
        vec2 q = p * 2.2 + vec2(fi * 5.0, fi * 3.0);
        float noise = 0.0;
        float amp = 0.5;

        for(int j = 0; j < 3; j++) {
            noise += amp * sin(q.x + cos(q.y * 0.8));
            q = mat2(0.8, -0.6, 0.6, 0.8) * q * 2.0 + vec2(2.1, 1.7);
            amp *= 0.5;
        }

        // Soft circular mask for smooth cloud boundaries
        float dist = length(p);
        float edgeFade = smoothstep(1.1, 0.35, dist);

        float density = smoothstep(0.1, 0.7, noise + 0.3) * edgeFade;

        vec3 dustColor = vec3(0.24, 0.27, 0.32);
        float brightness = 0.4 / depth;

        dustTotal += dustColor * density * brightness;
    }

    return dustTotal;
}