#pragma once

#include "swarcs/accretion/physics/Constants.hpp"

namespace swarcs::accretion::physics {

/**
 * @file Gravity.hpp
 * @brief Classical and Newtonian gravitational calculations.
 *
 * Provides methods for computing gravitational forces, accelerations, and field interactions
 * for standard celestial mechanics simulations before relativistic corrections are applied.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */
class Gravity {
public:
    // Static utility class — prevent instantiation
    Gravity() = delete;
    ~Gravity() = delete;
    Gravity(const Gravity&) = delete;
    Gravity& operator=(const Gravity&) = delete;

    /**
     * @brief Calculates the gravitational force magnitude between two masses.
     *
     * Formula: $F = G \cdot \frac{m_1 \cdot m_2}{r^2}$
     *
     * @param m1 Mass of the first body in kilograms (kg).
     * @param m2 Mass of the second body in kilograms (kg).
     * @param distance Distance between the centers of the two masses in meters (m).
     * @return double Gravitational force magnitude in Newtons (N). Returns 0.0 if distance <= 0.
     */
    [[nodiscard]] static constexpr double gravitationalForce(double m1, double m2, double distance) noexcept {
        if (distance <= 0.0) {
            return 0.0;
        }
        using namespace constants;
        return (GRAVITATIONAL_CONSTANT * m1 * m2) / (distance * distance);
    }

    /**
     * @brief Calculates the gravitational acceleration experienced at a given distance from a mass.
     *
     * Formula: $a = G \cdot \frac{M}{r^2}$
     *
     * @param mass Mass of the central body in kilograms (kg).
     * @param distance Distance from the center of mass in meters (m).
     * @return double Gravitational acceleration in meters per second squared ($m/s^2$). Returns 0.0 if distance <= 0.
     */
    [[nodiscard]] static constexpr double gravitationalAcceleration(double mass, double distance) noexcept {
        if (distance <= 0.0) {
            return 0.0;
        }
        using namespace constants;
        return (GRAVITATIONAL_CONSTANT * mass) / (distance * distance);
    }
};

} // namespace swarcs::accretion::physics