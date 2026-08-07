#version 300 es

/**
 * @file vertex.glsl
 * @brief Vertex shader transforming full-screen quad position attributes for OpenGL ES 3.0.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

layout (location = 0) in vec2 aPos;

void main() {
    // Pass raw normalized device coordinates directly to clip space
    gl_Position = vec4(aPos, 0.0, 1.0);
}