#pragma once

#include "swarcs/accretion/physics/Constants.hpp"

namespace swarcs::accretion::physics {

/**
 * @file Relativity.hpp
 * @brief Relativistic physics calculations for spacetime and black hole geometries.
 *
 * Implements foundational general relativity formulas, including Schwarzschild radius,
 * photon sphere boundaries, and innermost stable circular orbits (ISCO).
 *
 * @author Bosko Mijin
 * @since 2026-08
 */
class Relativity {
public:
    // Static utility class — prevent instantiation
    Relativity() = delete;
    ~Relativity() = delete;
    Relativity(const Relativity&) = delete;
    Relativity& operator=(const Relativity&) = delete;

    /**
     * @brief Calculates the Schwarzschild radius ($r_s$) for a given mass.
     *
     * Formula: $r_s = \frac{2GM}{c^2}$
     *
     * @param mass Mass of the celestial object in kilograms (kg).
     * @return double Schwarzschild radius in meters (m).
     */
    [[nodiscard]] static constexpr double schwarzschildRadius(double mass) noexcept {
        using namespace constants;
        return (2.0 * GRAVITATIONAL_CONSTANT * mass) / (SPEED_OF_LIGHT * SPEED_OF_LIGHT);
    }

    /**
     * @brief Calculates the photon sphere radius for a Schwarzschild black hole.
     *
     * For a non-rotating (Schwarzschild) black hole, the photon sphere lies at $1.5 \cdot r_s$.
     * Light rays can orbit the black hole at this exact radius.
     *
     * @param mass Mass of the black hole in kilograms (kg).
     * @return double Photon sphere radius in meters (m).
     */
    [[nodiscard]] static constexpr double photonSphereRadius(double mass) noexcept {
        return 1.5 * schwarzschildRadius(mass);
    }

    /**
     * @brief Calculates the Innermost Stable Circular Orbit (ISCO) radius.
     *
     * For a Schwarzschild black hole, the ISCO is located at $3 \cdot r_s$.
     * Particles cannot maintain stable circular orbits inside this radius.
     *
     * @param mass Mass of the black hole in kilograms (kg).
     * @return double ISCO radius in meters (m).
     */
    [[nodiscard]] static constexpr double iscoRadius(double mass) noexcept {
        return 3.0 * schwarzschildRadius(mass);
    }
};

} // namespace swarcs::accretion::physics