/**
 * @file constants.glsl
 * @brief Fundamental mathematical and physical constants for GLSL shaders.
 *
 * Defines core mathematical limits, geometric ratios, and normalized physical values
 * used across ray-marching, lighting, and relativistic calculations.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

/// Mathematical constant Pi ($\pi$).
const float PI = 3.14159265359;

/// Two times Pi ($2\pi$), representing a full circle in radians.
const float TWO_PI = 6.28318530718;

/// Half of Pi ($\pi / 2$), representing a right angle.
const float HALF_PI = 1.57079632679;

/// Normalized speed of light ($c$) scaled for ray-marching simulation space.
const float SPEED_OF_LIGHT = 1.0;