/**
 * @file nebula.glsl
 * @brief Procedural nebula cloud rendering following SRP.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Computes temporal drift and scaling for nebula coordinates.
 *
 * @param warpedUV Gravitationally distorted UV coordinates.
 * @param t Time variable for animation drift.
 * @return vec2 Transformed coordinate space.
 */
vec2 computeNebulaCoordinates(vec2 warpedUV, float t) {
    vec2 drift = vec2(sin(t * 0.1) * 0.08, cos(t * 0.07) * 0.08);
    return (warpedUV + drift) * 1.2;
}

/**
 * @brief Computes the density mask for cosmic dust and nebula clouds.
 *
 * @param p Transformed coordinate space.
 * @param t Time variable for octave drift.
 * @return float Smoothly stepped nebula mask factor.
 */
float computeNebulaMask(vec2 p, float t) {
    float n1 = fbm(p * 0.5);
    float n2 = fbm(p * 1.0 + vec2(5.2, 1.3) - t * 0.03);
    return smoothstep(0.3, 0.85, n1 * n2 * 1.8);
}

/**
 * @brief Computes rich deep space color tones based on noise sampling.
 *
 * @param p Transformed coordinate space.
 * @return vec3 Mixed RGB space color.
 */
vec3 computeNebulaColor(vec2 p) {
    float n1 = fbm(p * 0.5);
    vec3 nebulaColor1 = vec3(0.12, 0.02, 0.18);
    vec3 nebulaColor2 = vec3(0.02, 0.06, 0.15);
    return mix(nebulaColor1, nebulaColor2, n1);
}

/**
 * @brief Renders procedural cosmic dust and nebula clouds warped by gravity and animated over time.
 *
 * @param warpedUV Gravitationally distorted UV coordinates.
 * @param t Time variable for animation drift.
 * @return vec3 RGB color contribution of the nebula.
 */
vec3 renderNebula(vec2 warpedUV, float t) {
    vec2 p = computeNebulaCoordinates(warpedUV, t);
    float nebulaMask = computeNebulaMask(p, t);
    vec3 mixedNebula = computeNebulaColor(p);

    return mixedNebula * nebulaMask * 0.35;
}