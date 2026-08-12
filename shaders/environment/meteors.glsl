/**
 * @file meteors.glsl
 * @brief Ultra-fast, sharp shooting stars with thin needle-like trails.
 *
 * @author Bosko
 * @since 2026-08
 */

precision highp float;

/**
 * @brief Renders rapid shooting stars with an ultra-thin line and sharp point.
 *
 * @param uv Coordinates.
 * @param t Time variable for animation.
 * @return vec3 RGB color contribution of the meteors.
 */
vec3 renderMeteors(vec2 uv, float t) {
    vec3 meteorsTotal = vec3(0.0);

    // Much faster cycles so they flash and disappear quickly
    float meteorTime = t * 3.5;
    float meteorId = floor(meteorTime);
    float localT = fract(meteorTime);

    float n = hash21(vec2(meteorId, 73.1));
    if (n > 0.75) {
        vec2 startPos = vec2(
            hash21(vec2(meteorId, 1.1)) * 3.0 - 1.5,
            hash21(vec2(meteorId, 2.2)) * 3.0 - 1.5
        );
        vec2 direction = normalize(vec2(-2.0, -1.0));

        // Meteor head position
        vec2 headPos = startPos + direction * (localT * 4.0);

        vec2 toPixel = uv - headPos;
        vec2 tailDir = -direction;
        float proj = dot(toPixel, tailDir);

        // Shorter tail (0.2 instead of 0.5) and much sharper width falloff
        if (proj >= 0.0 && proj < 0.2) {
            vec2 perpVec = toPixel - tailDir * proj;
            float perpDist = length(perpVec);

            // Ultra-thin thread (increased coefficient from 150 to 600 for extreme thinness)
            float tailFactor = 1.0 - (proj / 0.2);
            float core = exp(-perpDist * 600.0) * pow(tailFactor, 3.0);

            // Sharp point on the head
            float headGlow = exp(-length(toPixel) * 800.0) * 3.0;

            // Quick fade-in and fade-out life cycle
            float lifeFade = sin(localT * 3.1415);

            vec3 meteorColor = vec3(0.9, 0.95, 1.0);
            meteorsTotal += meteorColor * (core * 1.2 + headGlow) * lifeFade;
        }
    }

    return meteorsTotal;
}