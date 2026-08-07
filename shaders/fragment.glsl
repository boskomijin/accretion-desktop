#version 300 es

/**
 * @file fragment.glsl
 * @brief Main fragment shader composition root for black hole accretion visualization.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

precision highp float;

out vec4 fragColor;

uniform float u_time;
uniform vec2 u_resolution;

// ==========================================
// 1. COMMON UTILITIES & ROTATIONS
// ==========================================
mat2 rotate(float angle) {
    float s = sin(angle), c = cos(angle);
    return mat2(c, -s, s, c);
}

// ==========================================
// 2. NOISE & FBM ALGORITHMS
// ==========================================
float hash21(vec2 p) {
    p = fract(p * vec2(234.34, 435.345));
    p += dot(p, p + 34.23);
    return fract(p.x * p.y);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(hash21(i + vec2(0.0, 0.0)), hash21(i + vec2(1.0, 0.0)), u.x),
               mix(hash21(i + vec2(0.0, 1.0)), hash21(i + vec2(1.0, 1.0)), u.x), u.y);
}

float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise(p * frequency);
        frequency *= 2.1;
        amplitude *= 0.48;
    }
    return value;
}

// ==========================================
// 3. BACKGROUND (STARS, PULSARS, NEBULA)
// ==========================================
vec3 renderBackgroundStars(vec2 uv, float t) {
    vec3 starsTotal = vec3(0.0);
    vec2 gridScale = uv * 12.0;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float clusterNoise = noise(cellId * 0.15);
    float threshold = mix(0.97, 0.85, clusterNoise);

    float n = hash21(cellId);
    if (n > threshold) {
        vec2 starPos = vec2(hash21(cellId + 1.2) - 0.5, hash21(cellId + 3.4) - 0.5) * 0.9;
        float dist = length(cellUv - starPos);
        float starSize = mix(0.008, 0.06, hash21(cellId + 5.6));
        float twinkle = sin(t * mix(0.5, 3.0, hash21(cellId + 8.1)) + hash21(cellId + 9.2) * 6.28) * 0.3 + 0.7;
        float brightness = mix(0.3, 1.5, hash21(cellId + 7.8)) * twinkle;

        float star = smoothstep(starSize, 0.0, dist) * brightness;
        vec3 starColor = mix(vec3(0.7, 0.8, 1.0), vec3(1.0, 0.9, 0.7), hash21(cellId + 9.0));
        starsTotal += starColor * star;
    }
    return starsTotal;
}

vec3 renderPulsars(vec2 warpedUV, float t) {
    vec3 pulsarsTotal = vec3(0.0);
    vec2 gridScale = warpedUV * 0.6;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float n = hash21(cellId * 97.1);
    if (n > 0.99) {
        vec2 pulsarPos = vec2(hash21(cellId + 1.1) - 0.5, hash21(cellId + 2.2) - 0.5) * 0.5;
        float dist = length(cellUv - pulsarPos);

        float pulseSpeed = mix(3.0, 6.0, hash21(cellId + 3.3));
        float pulse = pow(sin(t * pulseSpeed + hash21(cellId) * 10.0) * 0.5 + 0.5, 8.0);

        float core = (0.005 / (dist + 0.001)) * (pulse * 3.0 + 0.2);
        core = clamp(core, 0.0, 5.0);

        vec3 pulsarColor = mix(vec3(0.4, 0.7, 1.0), vec3(1.0, 0.4, 0.8), hash21(cellId + 4.4));
        pulsarsTotal += pulsarColor * core;
    }
    return pulsarsTotal;
}

vec3 renderNebula(vec2 warpedUV) {
    vec2 p = warpedUV * 1.2;
    float n1 = fbm(p * 0.5);
    float n2 = fbm(p * 1.0 + vec2(5.2, 1.3));
    float nebulaMask = smoothstep(0.4, 0.85, n1 * n2 * 1.5);
    vec3 nebulaBaseColor = vec3(0.03, 0.01, 0.02);
    return nebulaBaseColor * nebulaMask * 0.12;
}

// ==========================================
// 4. ACCRETION DISK & DOPPLER PHYSICS
// ==========================================
struct AccretionResult {
    vec3 color;
    float shape;
};

AccretionResult renderAccretionDisk(vec2 pBh, float eventHorizon, float t) {
    vec3 diskColor = vec3(0.0);
    mat2 diskRot = rotate(0.4);
    vec2 diskP = pBh * diskRot;
    float diskAngle = atan(diskP.y, diskP.x);
    diskP.y *= 3.0;
    float diskR = length(diskP);

    float diskDist = abs(diskR - 0.72);
    float diskShape = smoothstep(0.16, 0.0, diskDist);

    if (diskP.y > 0.0 && length(pBh) > eventHorizon && length(pBh) < 0.75) {
        diskShape += smoothstep(0.12, 0.0, abs(diskP.x)) * smoothstep(0.5, 0.1, length(pBh)) * 0.8;
    }

    float plasma = fbm(vec2(diskAngle * 2.5 - t * 1.2, (diskR - eventHorizon) * 4.0));

    float v = 0.85;
    float gamma = 1.0 / sqrt(1.0 - v * v);
    float doppler = (1.0 + v * cos(diskAngle)) / gamma;

    vec3 hotColor = mix(vec3(1.0, 0.45, 0.1), vec3(1.0, 0.8, 0.35), plasma);
    vec3 coldColor = mix(vec3(0.1, 0.4, 0.9), vec3(0.8, 0.2, 0.1), plasma * 0.3);
    vec3 finalGasColor = mix(hotColor, coldColor, clamp(doppler, 0.0, 1.0));

    diskColor = finalGasColor * diskShape * (plasma * 1.3 + 0.3) * (doppler * 2.1);

    float rBh = length(pBh);
    float ringDist = abs(rBh - (eventHorizon + 0.02));
    float organicRingFactor = smoothstep(0.025, 0.0, ringDist);
    float ringDoppler = sin(atan(pBh.y, pBh.x)) * 0.7 + 0.3;
    vec3 photonRingColor = mix(vec3(1.0, 0.6, 0.25), vec3(0.6, 0.85, 1.0), plasma) * 2.5;

    float ambientGlow = smoothstep(0.08, 0.0, abs(rBh - eventHorizon)) * 0.3;

    diskColor += photonRingColor * organicRingFactor * (plasma * 0.6 + 0.4) * (ringDoppler * 2.0);
    diskColor += vec3(0.9, 0.5, 0.2) * ambientGlow;

    return AccretionResult(diskColor, diskShape);
}

// ==========================================
// 5. MAIN COMPOSITION ROOT
// ==========================================
void main() {
    vec2 uv = (gl_FragCoord.xy - 0.5 * u_resolution.xy) / min(u_resolution.x, u_resolution.y);

    vec2 baseTranslation = vec2(sin(u_time * 0.02) * 0.05, cos(u_time * 0.015) * 0.05);
    uv += baseTranslation;
    uv *= rotate(u_time * 0.003);

    float t = u_time * 0.2;
    vec2 p = uv * 2.8;

    vec2 bhCenter = vec2(0.9, -0.7);
    vec2 pBh = p - bhCenter;
    float rBh = length(pBh);

    float eventHorizon = 0.22;
    float bend = eventHorizon / (rBh + 0.05);
    vec2 warpedUV = pBh * (1.0 + bend * 0.4);

    vec3 background = vec3(0.0);
    if (rBh > eventHorizon) {
        vec3 nebula = renderNebula(warpedUV);
        vec3 stars = renderBackgroundStars(warpedUV, t);
        vec3 pulsars = renderPulsars(warpedUV, t);
        background = nebula + stars + pulsars;
    }

    AccretionResult accretion = renderAccretionDisk(pBh, eventHorizon, t);

    vec3 col = vec3(0.0);
    col += background * (1.0 - min(accretion.shape, 0.95));
    col += accretion.color;

    float horizonMask = smoothstep(eventHorizon - 0.01, eventHorizon + 0.01, rBh);
    col *= horizonMask;

    col = pow(col, vec3(0.4545));
    col *= (1.0 - 0.28 * length(uv + vec2(-0.3, 0.2)));

    fragColor = vec4(col, 1.0);
}