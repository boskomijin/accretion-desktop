#version 300 es
/**
 * @file fragment.glsl
 * @brief Main fragment shader composition root for black hole accretion visualization.
 *
 * Orchestrates background stars, nebulae, pulsars, gravitational lens warping,
 * and the accretion disk rendering pipeline.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

#extension GL_GOOGLE_include_directive : enable

precision highp float;

out vec4 fragColor;

uniform float u_time;
uniform vec2 u_resolution;

// Include modular shader components
#include "common/noise.glsl"
#include "environment/nebula.glsl"
#include "environment/stars.glsl"
#include "astrophysics/accretion_disk.glsl"

void main() {
    // Normalize coordinates with aspect ratio preservation
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / min(u_resolution.x, u_resolution.y);

    // Apply slow global cinematic motion and rotation
    vec2 baseTranslation = vec2(sin(u_time * 0.02) * 0.05, cos(u_time * 0.015) * 0.05);
    uv += baseTranslation;
    uv *= rotate(u_time * 0.003);

    float t = u_time * 0.2;
    vec2 p = uv * 2.8;

    // Black hole center placement and coordinate space warping
    vec2 bhCenter = vec2(0.9, -0.7);
    vec2 pBh = p - bhCenter;
    float rBh = length(pBh);

    float eventHorizon = 0.22;
    float bend = eventHorizon / (rBh + 0.05);
    vec2 warpedUV = pBh * (1.0 + bend * 0.4);

    // Render background elements only outside the event horizon
    vec3 background = vec3(0.0);
    if (rBh > eventHorizon) {
        vec3 nebula = renderNebula(warpedUV);
        vec3 stars = renderBackgroundStars(warpedUV, t);
        vec3 pulsars = renderPulsars(warpedUV, t);
        background = nebula + stars + pulsars;
    }

    // Render accretion disk, Doppler effects, and photon ring
    AccretionResult accretion = renderAccretionDisk(pBh, eventHorizon, t);

    // Composite background and accretion disk layers
    vec3 col = vec3(0.0);
    col += background * (1.0 - min(accretion.shape, 0.95));
    col += accretion.color;

    // Apply event horizon black shadow mask
    float horizonMask = smoothstep(eventHorizon - 0.01, eventHorizon + 0.01, rBh);
    col *= horizonMask;

    // Gamma correction and subtle screen vignette shading
    col = pow(col, vec3(0.4545));
    col *= (1.0 - 0.28 * length(uv + vec2(-0.3, 0.2)));

    fragColor = vec4(col, 1.0);
}