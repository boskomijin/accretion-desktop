/**
 * @file accretion_disk.glsl
 * @brief High-fidelity accretion disk rendering with relativistic lensing effects.
 * 
 * Implements a primary accretion disk, depth-occluded rear hemisphere, and 
 * relativistic photon rings (crown) using Schwarzschild-like approximations.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

struct AccretionResult {
    vec3 color;
    float shape;
};

/**
 * @brief Renders the accretion disk and relativistic light rings.
 *
 * @param pBh Screen space position relative to the black hole center.
 * @param eventHorizon The radius of the event horizon.
 * @param t Time variable for animation.
 * @return AccretionResult The color and alpha-mask (shape) of the disk.
 */
AccretionResult renderAccretionDisk(vec2 pBh, float eventHorizon, float t) {
    vec3 diskColor = vec3(0.0);
    float rBh = length(pBh);
    
    // Discard rendering inside the event horizon
    if (rBh < eventHorizon) {
        return AccretionResult(vec3(0.0), 0.0);
    }

    // Disk perspective and rotation
    mat2 diskRot = rotate(0.38);
    vec2 diskP = pBh * diskRot;
    diskP.y *= 2.1; 
    
    float angle = atan(diskP.y, diskP.x);
    float distFromCenter = length(diskP);

    // Plasma turbulence (fbm)
    float fbm1 = fbm(vec2(angle * 3.0 - t * 0.8, distFromCenter * 3.5));
    float fbm2 = fbm(vec2(angle * 6.0 + t * 0.4, distFromCenter * 7.0));
    float cloudMix = clamp(fbm1 * 0.6 + fbm2 * 0.4, 0.0, 1.0);

    // Relativistic Doppler effect
    float v = 0.82;
    float gamma = 1.0 / sqrt(1.0 - v * v);
    float doppler = (1.0 + v * cos(angle)) / gamma;

    // Color palette
    vec3 hotColor = mix(vec3(1.0, 0.9, 0.7), vec3(1.0, 0.55, 0.2), cloudMix);
    vec3 coldColor = mix(vec3(0.55, 0.75, 1.0), vec3(0.2, 0.4, 0.8), cloudMix * 0.4);
    vec3 gasColor = mix(coldColor, hotColor, clamp(doppler * 0.7 + 0.3, 0.0, 1.0));

    // 1. Primary Disk Geometry
    float diskRadius = 0.68;
    float diskDist = abs(distFromCenter - diskRadius);
    float diskShape = smoothstep(0.2, 0.0, diskDist);
    float thickness = exp(-pow(diskP.y * 1.5, 2.0) * 4.0);
    
    // Mask rear hemisphere
    float backDiskMask = (diskP.y > -0.1) ? smoothstep(eventHorizon * 0.98, eventHorizon * 1.4, rBh) : 1.0;

    // 2. Relativistic Photon Rings (Upper and Lower Crowns)
    float crownRadius = eventHorizon * 1.08;
    float crownDist = abs(rBh - crownRadius);
    
    // Core calculation for high-frequency light ring
    float crownCore = exp(-pow(crownDist * 350.0, 1.8));
    
    // Vertical fading to prevent halo leakage
    float fadeTop = smoothstep(-0.01, 0.05, pBh.y) * smoothstep(1.2, 0.35, abs(pBh.x));
    float fadeBottom = smoothstep(-0.05, 0.01, -pBh.y) * smoothstep(1.2, 0.35, abs(pBh.x));
    
    float totalCrownShape = crownCore * (fadeTop + fadeBottom);
    vec3 crownColor = mix(vec3(0.85, 0.92, 1.0), vec3(1.0, 1.0, 1.0), cloudMix) * 7.0;

    // Composition
    vec3 mainDiskColor = gasColor * diskShape * thickness * (cloudMix * 0.8 + 0.4) * (doppler * 2.0) * backDiskMask;
    vec3 crownLayerColor = crownColor * totalCrownShape;

    diskColor = mainDiskColor + crownLayerColor;
    float totalShape = clamp((diskShape * backDiskMask) + totalCrownShape, 0.0, 1.0);

    return AccretionResult(diskColor, totalShape);
}