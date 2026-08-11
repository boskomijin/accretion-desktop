/**
 * @file photon_ring.glsl
 * @brief Integrated directly into accretion disk to avoid ring artifacts.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

struct PhotonRingResult {
    vec3 color;
    float shape;
};

PhotonRingResult renderPhotonRing(vec2 pBh, float eventHorizon, float t) {
    // Vraćamo nulu jer fotonski prsten i krunu sada radimo iz jedinstvenog diska bez vizuelnih grešaka
    return PhotonRingResult(vec3(0.0), 0.0);
}