/**
 * @file nebula.glsl
 * @brief Procedural nebula cloud rendering using multi-octave FBM noise.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders procedural cosmic dust and nebula clouds warped by gravity.
 *
 * @param warpedUV Gravitationally distorted UV coordinates.
 * @return vec3 RGB color contribution of the nebula.
 */
vec3 renderNebula(vec2 warpedUV) {
    vec2 p = warpedUV * 1.2;
    float n1 = fbm(p * 0.5);
    float n2 = fbm(p * 1.0 + vec2(5.2, 1.3));

    // Pojačana maska i kontrast magline
    float nebulaMask = smoothstep(0.3, 0.85, n1 * n2 * 1.8);

    // Bogatiji svemirski tonovi (duboka magenta/ljubičasta i plava)
    vec3 nebulaColor1 = vec3(0.12, 0.02, 0.18);
    vec3 nebulaColor2 = vec3(0.02, 0.06, 0.15);
    vec3 mixedNebula = mix(nebulaColor1, nebulaColor2, n1);

    return mixedNebula * nebulaMask * 0.35; // Vidljivije i lepše ukombinovano u pozadinu
}