#pragma once

namespace swarcs::accretion::physics::constants {

    /**
     * @file Constants.hpp
     * @brief Fundamental physical and astronomical constants in SI units.
     *
     * Provides core constants utilized across astrophysical models, orbital mechanics,
     * and relativistic ray-marching calculations.
     *
     * @author Bosko Mijin
     * @since 2026-08
     */

    /// Speed of light in vacuum ($c$), exact value in m/s.
    inline constexpr double SPEED_OF_LIGHT = 299792458.0;

    /// Newtonian gravitational constant ($G$), in $m^3 kg^{-1} s^{-2}$.
    inline constexpr double GRAVITATIONAL_CONSTANT = 6.67430e-11;

    /// Standard solar mass ($M_\odot$), in kilograms (kg).
    inline constexpr double SOLAR_MASS = 1.98847e30;

    /// Astronomical Unit ($AU$), standard earth-sun distance in meters (m).
    inline constexpr double ASTRONOMICAL_UNIT = 149597870700.0;

} // namespace swarcs::accretion::physics::constants