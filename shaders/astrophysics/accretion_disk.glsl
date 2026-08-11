/**
 * @file accretion_disk.glsl
 * @brief High-fidelity accretion disk rendering following SRP.
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
 * @brief Computes plasma turbulence mixing using multi-octave FBM.
 *
 * @param angle Angular coordinate along the disk.
 * @param distFromCenter Radial distance from the disk center.
 * @param t Time variable for animation.
 * @return float Normalized turbulence intensity mix.
 */
float computePlasmaTurbulence(float angle, float distFromCenter, float t) {
    float fbm1 = fbm(vec2(angle * 3.0 - t * 0.8, distFromCenter * 3.5));
    float fbm2 = fbm(vec2(angle * 6.0 + t * 0.4, distFromCenter * 7.0));
    return clamp(fbm1 * 0.6 + fbm2 * 0.4, 0.0, 1.0);
}

/**
 * @brief Calculates relativistic Doppler boosting and gas color palette.
 *
 * @param angle Angular coordinate for velocity vector projection.
 * @param cloudMix Turbulence intensity factor.
 * @return vec3 Resulting Doppler-shifted gas color.
 */
vec3 computeDopplerGasColor(float angle, float cloudMix) {
    float v = 0.82;
    float gamma = 1.0 / sqrt(1.0 - v * v);
    float doppler = (1.0 + v * cos(angle)) / gamma;

    vec3 hotColor = mix(vec3(1.0, 0.9, 0.7), vec3(1.0, 0.55, 0.2), cloudMix);
    vec3 coldColor = mix(vec3(0.55, 0.75, 1.0), vec3(0.2, 0.4, 0.8), cloudMix * 0.4);

    return mix(coldColor, hotColor, clamp(doppler * 0.7 + 0.3, 0.0, 1.0));
}

/**
 * @brief Renders the primary accretion disk geometry and back-hemisphere occlusion mask.
 *
 * @param diskP Rotated and stretched screen-space position.
 * @param pBh Original screen-space position relative to black hole center.
 * @param eventHorizon Radius of the event horizon.
 * @param distFromCenter Radial distance from disk center.
 * @param diskShape Output shape mask of the primary disk.
 * @param backDiskMask Output occlusion mask for the rear hemisphere.
 * @param thickness Output vertical thickness profile.
 */
void renderPrimaryDisk(vec2 diskP, vec2 pBh, float eventHorizon, float distFromCenter, out float diskShape, out float backDiskMask, out float thickness) {
    float diskRadius = 0.68;
    float diskDist = abs(distFromCenter - diskRadius);
    diskShape = smoothstep(0.2, 0.0, diskDist);
    thickness = exp(-pow(diskP.y * 1.5, 2.0) * 4.0);

    float rBh = length(pBh);
    backDiskMask = (diskP.y > -0.1) ? smoothstep(eventHorizon * 0.98, eventHorizon * 1.4, rBh) : 1.0;
}

/**
 * @brief Renders relativistic upper and lower photon rings (Gargantua crowns).
 *
 * @param pBh Screen-space position relative to black hole center.
 * @param eventHorizon Radius of the event horizon.
 * @param cloudMix Turbulence intensity factor for color variation.
 * @param crownColor Output color of the photon crowns.
 * @param totalCrownShape Output combined alpha mask of the crowns.
 */
void renderPhotonCrowns(vec2 pBh, float eventHorizon, float cloudMix, out vec3 crownColor, out float totalCrownShape) {
    float rBh = length(pBh);
    float crownRadius = eventHorizon * 1.08;
    float crownDist = abs(rBh - crownRadius);

    float crownCore = exp(-pow(crownDist * 350.0, 1.8));

    float fadeTop = smoothstep(-0.01, 0.05, pBh.y) * smoothstep(1.2, 0.35, abs(pBh.x));
    float fadeBottom = smoothstep(-0.05, 0.01, -pBh.y) * smoothstep(1.2, 0.35, abs(pBh.x));

    totalCrownShape = crownCore * (fadeTop + fadeBottom);
    crownColor = mix(vec3(0.85, 0.92, 1.0), vec3(1.0, 1.0, 1.0), cloudMix) * 7.0;
}

/**
 * @brief Main composition function orchestrating the accretion disk pipeline.
 *
 * @param pBh Screen space position relative to the black hole center.
 * @param eventHorizon The radius of the event horizon.
 * @param t Time variable for animation.
 * @return AccretionResult The color and alpha-mask (shape) of the disk.
 */
AccretionResult renderAccretionDisk(vec2 pBh, float eventHorizon, float t) {
    float rBh = length(pBh);
    if (rBh < eventHorizon) {
        return AccretionResult(vec3(0.0), 0.0);
    }

    mat2 diskRot = rotate(0.38);
    vec2 diskP = pBh * diskRot;
    diskP.y *= 2.1;

    float angle = atan(diskP.y, diskP.x);
    float distFromCenter = length(diskP);

    float cloudMix = computePlasmaTurbulence(angle, distFromCenter, t);
    vec3 gasColor = computeDopplerGasColor(angle, cloudMix);

    float diskShape, backDiskMask, thickness;
    renderPrimaryDisk(diskP, pBh, eventHorizon, distFromCenter, diskShape, backDiskMask, thickness);

    vec3 crownColor;
    float totalCrownShape;
    renderPhotonCrowns(pBh, eventHorizon, cloudMix, crownColor, totalCrownShape);

    float v = 0.82;
    float gamma = 1.0 / sqrt(1.0 - v * v);
    float doppler = (1.0 + v * cos(angle)) / gamma;

    vec3 mainDiskColor = gasColor * diskShape * thickness * (cloudMix * 0.8 + 0.4) * (doppler * 2.0) * backDiskMask;
    vec3 crownLayerColor = crownColor * totalCrownShape;

    vec3 diskColor = mainDiskColor + crownLayerColor;
    float totalShape = clamp((diskShape * backDiskMask) + totalCrownShape, 0.0, 1.0);

    return AccretionResult(diskColor, totalShape);
}