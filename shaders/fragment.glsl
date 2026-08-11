#version 300 es
/**
 * @file fragment.glsl
 * @brief Interstellar-style black hole visualization with accurate color balance.
 *
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

void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / min(u_resolution.x, u_resolution.y);
    vec3 ray = normalize(vec3(uv, 5.0));

    // Suptilna rotacija kamere
    float yaw = sin(u_time * 0.08) * 0.04;
    ray.xz = mat2(cos(yaw), -sin(yaw), sin(yaw), cos(yaw)) * ray.xz;

    float pitch = cos(u_time * 0.06) * 0.03;
    ray.yz = mat2(cos(pitch), -sin(pitch), sin(pitch), cos(pitch)) * ray.yz;

    // Definisanje crne rupe
    BlackHole bh;
    bh.position = vec3(2.4, 0.0, 0.0);
    bh.mass = 1.0;
    bh.schwarzschildRadius = 0.22;

    // Gravitaciono sočivo
    LensingResult lensing = calculateGravitationalLensing(vec3(0.0), ray, bh);
    vec3 deflectedRay = lensing.direction;

    vec2 p = (deflectedRay.xy / deflectedRay.z) * 26.0;
    vec2 pBh = p - bh.position.xy;
    float rBh = length(pBh);

    float t_fast = u_time * 0.3;
    float t_slow = u_time * 0.05;

    // Pozadina i nebula (bez veštačkog razlivanja sivog prstena)
    vec2 cosmicDrift = vec2(sin(t_slow * 0.3) * 0.03, cos(t_slow * 0.2) * 0.03);
    vec2 backgroundUV = p + cosmicDrift;

    vec3 background = vec3(0.0);
    if (rBh > bh.schwarzschildRadius) {
        background = renderNebula(backgroundUV, t_slow)
                   + renderBackgroundStars(backgroundUV, t_slow)
                   + renderPulsars(backgroundUV, t_slow);
    }

    // Akrecioni disk i fotonski prsten
    AccretionResult accretion = renderAccretionDisk(pBh, bh.schwarzschildRadius, t_fast);
    PhotonRingResult ring = renderPhotonRing(pBh, bh.schwarzschildRadius, t_fast);

    // Kompozicija slojeva
    vec3 col = vec3(0.0);
    col += background * (1.0 - min(accretion.shape + ring.shape, 0.95));
    col += accretion.color;
    col += ring.color;

    // Fini, drastično stanjeni difrakcioni halo uz samu ivicu (bez sivog oreola)
    float flare = exp(-pow(rBh - bh.schwarzschildRadius, 2.0) * 1200.0);
    vec3 flareColor = vec3(0.8, 0.9, 1.0);
    col += flareColor * flare * 0.15;

    // Maska horizonta događaja (apsolutna crna sena bez curenja boja)
    float horizonMask = smoothstep(bh.schwarzschildRadius - 0.003, bh.schwarzschildRadius + 0.003, rBh);
    col *= horizonMask;

    // Gamma korekcija + vinjeta
    col = pow(col, vec3(0.4545));
    col *= (1.0 - 0.28 * length(uv));

    fragColor = vec4(col, 1.0);
}