#version 300 es
/**
 * @file vertex.glsl
 * @brief Full-screen triangle vertex shader using gl_VertexID following SRP and clean architecture.
 *
 * @author Bosko Mijin
 * @since 2026-02
 */

precision highp float;

/**
 * @brief Computes screen-space vertex coordinates for a full-screen triangle using gl_VertexID.
 *
 * @param vertexId Built-in vertex identifier.
 * @return vec2 Normalized device coordinates for the vertex.
 */
vec2 computeFullscreenTrianglePosition(int vertexId) {
    float x = float((vertexId & 1) << 2);
    float y = float((vertexId & 2) << 1);
    return vec2(x - 1.0, y - 1.0);
}

void main() {
    vec2 ndcPos = computeFullscreenTrianglePosition(gl_VertexID);
    gl_Position = vec4(ndcPos, 0.0, 1.0);
}