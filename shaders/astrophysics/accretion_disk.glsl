/**
 * @file accretion_disk.glsl
 * @brief Accretion disk rendering, plasma turbulence, Doppler boosting, and photon ring physics.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @struct AccretionResult
 * @brief Holds the composite color and geometric shape factor of the disk.
 */
struct AccretionResult {
    vec3 color;
    float shape;
};

/**
 * @brief Renders the accretion disk, plasma turbulence, Doppler effect, and photon ring.
 *
 * @param pBh Position relative to the black hole center in warped space.
 * @param eventHorizon Radius of the black hole event horizon.
 * @param t Time variable for plasma animation.
 * @return AccretionResult Resulting color and opacity/shape mask.
 */
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