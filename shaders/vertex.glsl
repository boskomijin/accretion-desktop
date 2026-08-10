#version 300 es
/**
 * @file vertex.glsl
 * @brief Full-screen triangle vertex shader using gl_VertexID.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

precision highp float;

void main() {
    // Generates a single large triangle covering the entire screen (-1 to 3) using gl_VertexID
    float x = float((gl_VertexID & 1) << 2);
    float y = float((gl_VertexID & 2) << 1);

    gl_Position = vec4(x - 1.0, y - 1.0, 0.0, 1.0);
}