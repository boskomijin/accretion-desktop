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
    const vec2 positions[3] = vec2[](
            vec2(-1.0, -1.0),
            vec2( 3.0, -1.0),
            vec2(-1.0,  3.0)
        );

        gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
}