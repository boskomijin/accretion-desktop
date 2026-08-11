/**
 * @file black_hole.glsl
 * @brief General relativity and black hole lensing calculations.
 *
 * Implements Schwarzschild geometry approximations for ray deflection
 * and event horizon capture.
 *
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
    vec3 toCenter = bh.position - rayPos;
    float dist = length(toCenter);

    // Capture check
    if (dist < bh.schwarzschildRadius) {
        return LensingResult(vec3(0.0), 1.0);
    }

    // Impact parameter (approximate)
    float b = length(cross(rayPos - bh.position, rayDir)) / length(rayDir);

    // Deflection angle
    float impactFactor = smoothstep(bh.schwarzschildRadius, bh.schwarzschildRadius * 3.0, b);
    float alpha = ((2.0 * bh.schwarzschildRadius) / (b + 0.001)) * (1.0 - impactFactor);

    // Apply deflection
    vec3 deflectedDir = normalize(rayDir + normalize(toCenter) * alpha);

    return LensingResult(deflectedDir, 0.0);
}
