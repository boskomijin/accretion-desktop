/**
 * @file black_hole.glsl
 * @brief General relativity and black hole lensing calculations following SRP.
 *
 * Implements Schwarzschild geometry approximations for ray deflection
 * and event horizon capture.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @struct BlackHole
 * @brief Represents a non-rotating (Schwarzschild) black hole object in the scene.
 */
struct BlackHole {
    vec3 position;             ///< World-space position of the black hole center.
    float mass;                ///< Mass parameter (scaled).
    float schwarzschildRadius; ///< Schwarzschild radius (event horizon).
};

/**
 * @struct LensingResult
 * @brief Holds deflected ray direction and capture mask.
 */
struct LensingResult {
    vec3 direction; ///< Deflected ray direction.
    float captured; ///< 1.0 if ray is captured, 0.0 otherwise.
};

/**
 * @brief Checks if a light ray intersects and gets trapped by the event horizon.
 *
 * @param rayPos Current position of the light ray in world space.
 * @param bh BlackHole object defining the event horizon.
 * @return bool True if the ray is trapped inside the horizon.
 */
bool isRayCaptured(vec3 rayPos, BlackHole bh) {
    float dist = length(bh.position - rayPos);
    return dist < bh.schwarzschildRadius;
}

/**
 * @brief Computes the impact parameter for gravitational ray bending.
 *
 * @param rayPos Current position of the light ray.
 * @param rayDir Normalized direction vector of the ray.
 * @param bh BlackHole object center reference.
 * @return float Calculated impact parameter magnitude.
 */
float computeImpactParameter(vec3 rayPos, vec3 rayDir, BlackHole bh) {
    return length(cross(rayPos - bh.position, rayDir)) / length(rayDir);
}

/**
 * @brief Computes the light ray deflection alpha angle based on impact factor.
 *
 * @param b Impact parameter.
 * @param schwarzschildRadius Schwarzschild radius of the black hole.
 * @return float Computed deflection angle magnitude.
 */
float computeDeflectionAngle(float b, float schwarzschildRadius) {
    float impactFactor = smoothstep(schwarzschildRadius, schwarzschildRadius * 3.0, b);
    return ((2.0 * schwarzschildRadius) / (b + 0.001)) * (1.0 - impactFactor);
}

/**
 * @brief Computes gravitational lensing using Schwarzschild approximation.
 *
 * Uses impact parameter and deflection angle approximation:
 *   alpha ≈ 2 * r_s / b
 *
 * @param rayPos Current position of the light ray in world space.
 * @param rayDir Current normalized direction vector of the ray.
 * @param bh BlackHole object influencing the ray.
 * @return LensingResult Deflected ray and capture mask.
 */
LensingResult calculateGravitationalLensing(vec3 rayPos, vec3 rayDir, BlackHole bh) {
    if (isRayCaptured(rayPos, bh)) {
        return LensingResult(vec3(0.0), 1.0);
    }

    vec3 toCenter = bh.position - rayPos;
    float b = computeImpactParameter(rayPos, rayDir, bh);
    float alpha = computeDeflectionAngle(b, bh.schwarzschildRadius);

    vec3 deflectedDir = normalize(rayDir + normalize(toCenter) * alpha);

    return LensingResult(deflectedDir, 0.0);
}