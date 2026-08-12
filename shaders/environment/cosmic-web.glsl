/**
 * @file cosmic-web.glsl
 * @brief Soft, organic cosmic web filaments for deep space background texture.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders diffuse cosmic web filaments using smooth multi-octave noise.
 *
 * @param warpedUV Warped screen coordinates.
 * @param t Time variable for extremely slow drift.
 * @return vec3 RGB color contribution of the cosmic web.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */
vec3 renderCosmicWeb(vec2 warpedUV, float t) {
    // Slow, deep spatial drift
    vec2 driftUV = warpedUV + vec2(-t * 0.00004, t * 0.00003);

    vec2 p = driftUV * 0.7;

    // Organic space warping rotation
    float angle = 0.5;
    mat2 rot = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));

    float n = 0.0;
    float amp = 0.5;

    // Multi-layer smooth noise accumulation instead of sharp sine lines
    for(int i = 0; i < 3; i++) {
        p = rot * p * 1.5 + vec2(1.2, 1.7);
        n += amp * (sin(p.x + cos(p.y * 1.5)) * 0.5 + 0.5);
        amp *= 0.5;
    }

    // Extremely soft transition to blend seamlessly with the background without harsh edges
    float webIntensity = smoothstep(0.3, 0.9, n) * 0.08;

    // Deep, cold bluish-purple tone
    vec3 webColor = vec3(0.14, 0.20, 0.32);

    return webColor * webIntensity;
}