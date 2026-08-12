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
#include "environment/meteors.glsl"
#include "environment/nebula.glsl"
#include "environment/stars.glsl"
#include "environment/pulsars.glsl"
#include "environment/quasars.glsl"
#include "astrophysics/accretion_disk.glsl"
#include "astrophysics/photon_ring.glsl"
#include "astrophysics/black_hole.glsl"

/**
 * @brief Context structure for render pipeline to maintain SRP and clean architecture.
 */
struct RenderContext {
    vec2 uv;
    vec2 p;
    vec2 pBh;
    float rBh;
    vec3 ray;
    BlackHole bh;
};

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
 * @brief Prepares the render context including camera ray, gravitational lensing, and coordinate projection.
 *
 * @param fragCoord Fragment coordinates from screen space.
 * @return RenderContext Initialized render context.
 */
RenderContext prepareRenderContext(vec2 fragCoord) {
    RenderContext ctx;
    ctx.uv = (fragCoord - 0.5 * u_resolution.xy) / min(u_resolution.x, u_resolution.y);
    ctx.ray = computeCameraRay(ctx.uv, u_time);
    ctx.bh = initBlackHole();

    LensingResult lensing = calculateGravitationalLensing(vec3(0.0), ctx.ray, ctx.bh);
    vec3 deflectedRay = lensing.direction;

    ctx.p = (deflectedRay.xy / deflectedRay.z) * 26.0;
    ctx.pBh = ctx.p - ctx.bh.position.xy;
    ctx.rBh = length(ctx.pBh);

    return ctx;
}

/**
 * @brief Renders background cosmic environment (nebula, stars, pulsars, quasars) outside the event horizon.
 *
 * @param ctx Current render context.
 * @param t_slow Time variable for slow background drift.
 * @return vec3 Computed background color.
 */
vec3 renderCosmicBackground(RenderContext ctx, float t_slow) {
    if (ctx.rBh <= ctx.bh.schwarzschildRadius) {
        return vec3(0.0);
    }

    vec2 cosmicDrift = vec2(sin(t_slow * 0.3) * 0.03, cos(t_slow * 0.2) * 0.03);
    vec2 backgroundUV = ctx.p + cosmicDrift;

    return renderNebula(backgroundUV, t_slow)
         + renderBackgroundStars(backgroundUV, t_slow)
         + renderConstellations(backgroundUV, t_slow)
         + renderPulsars(backgroundUV, t_slow)
         + renderQuasars(backgroundUV, t_slow)
         + renderMeteors(backgroundUV, t_slow);
}

/**
 * @brief Composes all visual scene layers (background, accretion disk, photon ring).
 *
 * @param ctx Current render context.
 * @return vec3 Composite layer color.
 */
vec3 composeSceneLayers(RenderContext ctx) {
    float t_fast = u_time * 0.3;
    float t_slow = u_time * 0.05;

    vec3 background = renderCosmicBackground(ctx, t_slow);
    AccretionResult accretion = renderAccretionDisk(ctx.pBh, ctx.bh.schwarzschildRadius, t_fast);
    PhotonRingResult ring = renderPhotonRing(ctx.pBh, ctx.bh.schwarzschildRadius, t_fast);

    vec3 col = vec3(0.0);
    col += background * (1.0 - min(accretion.shape + ring.shape, 0.95));
    col += accretion.color;
    col += ring.color;

    return col;
}

/**
 * @brief Applies edge diffraction flare and event horizon black-out mask.
 *
 * @param col Current composite color.
 * @param ctx Current render context.
 * @return vec3 Masked and flared color.
 */
vec3 applyBlackHoleMaskAndFlare(vec3 col, RenderContext ctx) {
    float flare = exp(-pow(ctx.rBh - ctx.bh.schwarzschildRadius, 2.0) * 1200.0);
    vec3 flareColor = vec3(0.8, 0.9, 1.0);
    col += flareColor * flare * 0.15;

    float horizonMask = smoothstep(ctx.bh.schwarzschildRadius - 0.003, ctx.bh.schwarzschildRadius + 0.003, ctx.rBh);
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
    RenderContext ctx = prepareRenderContext(gl_FragCoord.xy);

    vec3 col = composeSceneLayers(ctx);
    col = applyBlackHoleMaskAndFlare(col, ctx);
    col = applyPostProcessing(col, ctx.uv);

    fragColor = vec4(col, 1.0);
}