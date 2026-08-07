#include "swarcs/accretion/graphics/FullScreenQuad.hpp"

namespace swarcs::accretion::graphics {

    /**
     * @brief Constructs the full-screen quad by defining vertices and uploading them to the GPU.
     *
     * Allocates a VBO and populates it with standard 2D NDC coordinates covering the entire screen.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    FullScreenQuad::FullScreenQuad() {
        // Define two triangles forming a rectangle covering normalized device coordinates (-1.0 to 1.0)
        float vertices[] = {
            -1.0f,  1.0f,
            -1.0f, -1.0f,
             1.0f, -1.0f,

            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f
        };

        // Generate, bind, and upload vertex data to the VBO buffer on the GPU
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /**
     * @brief Cleans up GPU buffer resources upon destruction.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    FullScreenQuad::~FullScreenQuad() {
        if (vbo != 0) {
            glDeleteBuffers(1, &vbo);
        }
    }

    /**
     * @brief Binds the quad's vertex array attributes and renders it using triangles.
     *
     * @author Bosko Mijin
     * @since 2026-02
     */
    void FullScreenQuad::draw() const {
        // Bind the vertex buffer object
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Configure vertex attribute pointers for position layout
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        // Issue draw call to render the 6 vertices (2 triangles)
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Clean up attribute state and unbind buffer
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace swarcs::accretion::graphics