/**
 * @file photon_ring.glsl
 * @brief Integrated directly into accretion disk to avoid ring artifacts.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

/**
 * @struct PhotonRingResult
 * @brief Holds the composite color and shape mask for the standalone photon ring module.
 */
struct PhotonRingResult {
    vec3 color; ///< RGB color contribution.
    float shape; ///< Alpha/shape mask factor.
};

/**
 * @brief Stubbed render function returning zero to prevent artifacts,
 *        as photon crowns are integrated directly within the accretion disk pipeline.
 *
 * @param pBh Screen space position relative to the black hole center.
 * @param eventHorizon Radius of the black hole event horizon.
 * @param t Time variable for animation.
 * @return PhotonRingResult Zero-initialized result container.
 */
PhotonRingResult renderPhotonRing(vec2 pBh, float eventHorizon, float t) {
    // Vraćamo nulu jer fotonski prsten i krunu sada radimo iz jedinstvenog diska bez vizuelnih grešaka
    return PhotonRingResult(vec3(0.0), 0.0);
}