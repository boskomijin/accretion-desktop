/**
 * @file black_hole.glsl
 * @brief General relativity and black hole lensing calculations.
 *
 * Implements data structures and gravitational deflection equations for light rays
 * passing near a Schwarzschild black hole.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

/**
 * @struct BlackHole
 * @brief Represents a non-rotating (Schwarzschild) black hole object in the scene.
 */
struct BlackHole {
    vec3 position;             ///< World-space position of the black hole center.
    float mass;                ///< Scaled mass parameter governing gravitational field strength.
    float schwarzschildRadius; ///< Schwarzschild radius ($r_s = 2GM/c^2$) defining the event horizon.
};

/**
 * @brief Computes gravitational lensing (spacetime curvature effect on light rays).
 *
 * Approximates light ray bending toward the gravitational center and evaluates
 * whether the ray crosses the event horizon.
 *
 * @param rayPos Current position of the light ray in world space.
 * @param rayDir Current normalized direction vector of the ray.
 * @param bh Reference to the BlackHole object influencing the ray.
 * @return vec3 Deflected ray direction vector, or vec3(0.0) if captured by the event horizon.
 */
vec3 calculateGravitationalLensing(vec3 rayPos, vec3 rayDir, BlackHole bh) {
    vec3 toCenter = bh.position - rayPos;
    float dist = length(toCenter);

    // Check if the ray has crossed the event horizon (trapped inside the black hole)
    if (dist < bh.schwarzschildRadius) {
        return vec3(0.0);
    }

    // Approximate gravitational deflection inversely proportional to the square of the distance
    float bendFactor = (1.5 * bh.schwarzschildRadius) / (dist * dist);
    vec3 deflectedDir = normalize(rayDir + normalize(toCenter) * bendFactor);

    return deflectedDir;
}