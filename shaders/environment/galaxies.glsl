/**
 * @file galaxies.glsl
 * @brief Procedural distant galaxies with smooth, natural spiral arms and realistic distribution.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders natural background galaxies using spatial cell hashing and smooth spiral structuring.
 *
 * @param warpedUV Warped coordinates.
 * @param t Time variable for slow galactic rotation and drift.
 * @return vec3 RGB color contribution of the galaxies.
 */
vec3 renderGalaxies(vec2 warpedUV, float t) {
    vec2 driftUV = warpedUV + vec2(t * 0.0003, -t * 0.0006);

    vec3 galaxiesTotal = vec3(0.0);

    vec2 gridScale = driftUV * 0.5;
    vec2 cellId = floor(gridScale);
    vec2 cellUv = fract(gridScale) - 0.5;

    float n = hash21(cellId * 87.4 + 14.1);
    if (n > 0.78) {
        vec2 galaxyPos = vec2(hash21(cellId + 3.2) - 0.5, hash21(cellId + 4.5) - 0.5) * 0.7;
        vec2 toGalaxy = cellUv - galaxyPos;

        float sizeRand = hash21(cellId + 9.1);
        float scaleFactor = mix(0.6, 1.6, sizeRand);
        float id = hash21(cellId + 7.8);
        float rotAngle = t * 0.0015 + id * 6.28;

        mat2 rot = mat2(cos(rotAngle), -sin(rotAngle), sin(rotAngle), cos(rotAngle));
        vec2 p = rot * toGalaxy * scaleFactor;

        // Elliptic disk tilt
        float tilt = mix(1.5, 2.5, hash21(cellId + 11.4));
        p.y *= tilt;

        float dist = length(p);

        // Natural, soft core
        float core = smoothstep(0.12 * scaleFactor, 0.0, dist) * 1.2;

        // Subtle spiral arms
        float angle = atan(p.y, p.x);
        float spiral = sin(angle * 2.0 - dist * 10.0 + t * 0.004) * 0.5 + 0.5;

        // Disk with smooth edge falloff
        float diskSize = 0.3 * scaleFactor;
        float disk = smoothstep(diskSize, 0.02, dist) * (spiral * 0.7 + 0.3);

        float slowFade = sin(t * 0.15 + id * 6.28) * 0.15 + 0.85;
        vec3 galColor = mix(vec3(0.7, 0.8, 1.0), vec3(1.0, 0.85, 0.7), id);

        float depthBrightness = 0.35 / scaleFactor;
        galaxiesTotal += galColor * (core + disk * 0.4) * slowFade * depthBrightness;
    }

    return galaxiesTotal;
}