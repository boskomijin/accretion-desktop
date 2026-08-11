#version 300 es
/**
 * @file fragment.glsl
 * @brief Interstellar-style black hole visualization following SRP and clean architecture.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

out vec4 fragColor;

uniform float u_time;
uniform vec2 u_resolution;

#include "common/constants.glsl"
#include "common/math.glsl"
#include "common/noise.glsl"
#include "environment/nebula.glsl"
#include "environment/stars.glsl"
#include "astrophysics/accretion_disk.glsl"
#include "astrophysics/photon_ring.glsl"
#include "astrophysics/black_hole.glsl"

/**
 * @brief Computes normalized camera view ray with subtle rotational drift.
 *
 * @param uv Normalized screen coordinates.
 * @param time Current scene time.
 * @return vec3 Rotated camera ray direction.
 */
vec3 computeCameraRay(vec2 uv, float time) {
    vec3 ray = normalize(vec3(uv, 5.0));

    float yaw = sin(time * 0.08) * 0.04;
    ray.xz = mat2(cos(yaw), -sin(yaw), sin(yaw), cos(yaw)) * ray.xz;

    float pitch = cos(time * 0.06) * 0.03;
    ray.yz = mat2(cos(pitch), -sin(pitch), sin(pitch), cos(pitch)) * ray.yz;

    return ray;
}

/**
 * @brief Initializes the Schwarzschild black hole scene parameters.
 *
 * @return BlackHole Configured black hole instance.
 */
BlackHole initBlackHole() {
    BlackHole bh;
    bh.position = vec3(2.4, 0.0, 0.0);
    bh.mass = 1.0;
    bh.schwarzschildRadius = 0.22;
    return bh;
}

/**
 * @brief Renders background cosmic environment (nebula, stars, pulsars) outside the event horizon.
 *
 * @param rBh Radial distance from black hole center.
 * @param schwarzschildRadius Radius of the event horizon.
 * @param backgroundUV Warped background UV coordinates.
 * @param t_slow Time variable for slow background drift.
 * @return vec3 Computed background color.
 */
vec3 renderCosmicBackground(float rBh, float schwarzschildRadius, vec2 backgroundUV, float t_slow) {
    if (rBh <= schwarzschildRadius) {
        return vec3(0.0);
    }

    return renderNebula(backgroundUV, t_slow)
         + renderBackgroundStars(backgroundUV, t_slow)
         + renderPulsars(backgroundUV, t_slow);
}

/**
 * @brief Applies edge diffraction flare and event horizon black-out mask.
 *
 * @param col Current composite color.
 * @param rBh Radial distance from black hole center.
 * @param schwarzschildRadius Radius of the event horizon.
 * @return vec3 Masked and flared color.
 */
vec3 applyBlackHoleMaskAndFlare(vec3 col, float rBh, float schwarzschildRadius) {
    float flare = exp(-pow(rBh - schwarzschildRadius, 2.0) * 1200.0);
    vec3 flareColor = vec3(0.8, 0.9, 1.0);
    col += flareColor * flare * 0.15;

    float horizonMask = smoothstep(schwarzschildRadius - 0.003, schwarzschildRadius + 0.003, rBh);
    return col * horizonMask;
}

/**
 * @brief Applies final post-processing including gamma correction and screen vignette.
 *
 * @param col Composite color input.
 * @param uv Normalized screen coordinates.
 * @return vec3 Final post-processed color.
 */
vec3 applyPostProcessing(vec3 col, vec2 uv) {
    col = pow(col, vec3(0.4545));
    col *= (1.0 - 0.28 * length(uv));
    return col;
}

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / min(u_resolution.x, u_resolution.y);
    vec3 ray = computeCameraRay(uv, u_time);

    BlackHole bh = initBlackHole();

    LensingResult lensing = calculateGravitationalLensing(vec3(0.0), ray, bh);
    vec3 deflectedRay = lensing.direction;

    vec2 p = (deflectedRay.xy / deflectedRay.z) * 26.0;
    vec2 pBh = p - bh.position.xy;
    float rBh = length(pBh);

    float t_fast = u_time * 0.3;
    float t_slow = u_time * 0.05;

    vec2 cosmicDrift = vec2(sin(t_slow * 0.3) * 0.03, cos(t_slow * 0.2) * 0.03);
    vec2 backgroundUV = p + cosmicDrift;

    vec3 background = renderCosmicBackground(rBh, bh.schwarzschildRadius, backgroundUV, t_slow);

    AccretionResult accretion = renderAccretionDisk(pBh, bh.schwarzschildRadius, t_fast);
    PhotonRingResult ring = renderPhotonRing(pBh, bh.schwarzschildRadius, t_fast);

    vec3 col = vec3(0.0);
    col += background * (1.0 - min(accretion.shape + ring.shape, 0.95));
    col += accretion.color;
    col += ring.color;

    col = applyBlackHoleMaskAndFlare(col, rBh, bh.schwarzschildRadius);
    col = applyPostProcessing(col, uv);

    fragColor = vec4(col, 1.0);
}