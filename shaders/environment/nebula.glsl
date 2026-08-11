/**
 * @file nebula.glsl
 * @brief Procedural nebula cloud rendering using multi-octave FBM noise with temporal drift.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders procedural cosmic dust and nebula clouds warped by gravity and animated over time.
 *
 * @param warpedUV Gravitationally distorted UV coordinates.
 * @param t Time variable for animation drift.
 * @return vec3 RGB color contribution of the nebula.
 */
vec3 renderNebula(vec2 warpedUV, float t) {
    // Subtle procedural drift over time so clouds slowly shift and evolve
    vec2 drift = vec2(sin(t * 0.1) * 0.08, cos(t * 0.07) * 0.08);
    vec2 p = (warpedUV + drift) * 1.2;

    float n1 = fbm(p * 0.5);
    float n2 = fbm(p * 1.0 + vec2(5.2, 1.3) - t * 0.03);

    // Enhanced mask and contrast for deep cosmic depth
    float nebulaMask = smoothstep(0.3, 0.85, n1 * n2 * 1.8);

    // Rich deep space tones (magenta/purple and deep blue mix)
    vec3 nebulaColor1 = vec3(0.12, 0.02, 0.18);
    vec3 nebulaColor2 = vec3(0.02, 0.06, 0.15);
    vec3 mixedNebula = mix(nebulaColor1, nebulaColor2, n1);

    return mixedNebula * nebulaMask * 0.35;
}