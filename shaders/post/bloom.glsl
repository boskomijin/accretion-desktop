#version 300 es
/**
 * @file bloom.glsl
 * @brief Post-processing bloom and tone mapping filter for cinematic lighting.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

precision highp float;

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D uSceneTexture;
uniform vec2 uResolution;
uniform float uBloomIntensity;
uniform float uExposure;

void main() {
    // Prevent division by zero if resolution is uninitialized
    vec2 res = (uResolution.x <= 0.0 || uResolution.y <= 0.0) ? vec2(3840.0, 1080.0) : uResolution;
    vec2 uv = gl_FragCoord.xy / res;

    vec4 color = texture(uSceneTexture, uv);

    // Extract bright areas (threshold) for bloom effect
    vec3 brightColor = vec3(0.0);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 0.6) {
        brightColor = color.rgb * (brightness - 0.6);
    }

    // Simple cross-sample blur for ambient glow
    vec2 texelSize = 1.0 / res;
    vec3 bloomSum = vec3(0.0);
    for(int x = -2; x <= 2; x++) {
        for(int y = -2; y <= 2; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize * 1.5;
            vec3 sampleColor = texture(uSceneTexture, uv + offset).rgb;
            float sampleBright = dot(sampleColor, vec3(0.2126, 0.7152, 0.0722));
            if(sampleBright > 0.5) {
                bloomSum += sampleColor * (sampleBright - 0.5);
            }
        }
    }
    bloomSum /= 25.0;

    // Fallback values if uniforms are not passed from C++ (prevents black screen)
    float intensity = (uBloomIntensity <= 0.0) ? 0.4 : uBloomIntensity;
    float exposure = (uExposure <= 0.0) ? 1.0 : uExposure;

    // Combine scene color with bloom effect
    vec3 finalColor = color.rgb + bloomSum * intensity;

    // Tone mapping (Exposure)
    finalColor = vec3(1.0) - exp(-finalColor * exposure);

    // Subtle vignette effect (darkening screen corners)
    vec2 centerUV = uv - 0.5;
    float vignette = 1.0 - dot(centerUV, centerUV) * 0.4;
    finalColor *= vignette;

    fragColor = vec4(finalColor, color.a);
}