/**
 * @file FrameBuffer.cpp
 * @brief Implementation of OpenGL Framebuffer Object for multi-pass rendering.
 *
 * @author Bosko Mijin
 * @since 2026-08
 */

#include "swarcs/accretion/graphics/FrameBuffer.hpp"
#include <stdexcept>

namespace swarcs::accretion::graphics {

FrameBuffer::FrameBuffer(int w, int h) : width(w), height(h) {
    create(width, height);
}

FrameBuffer::~FrameBuffer() {
    cleanup();
}

void FrameBuffer::create(int w, int h) {
    // 1. Create FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // 2. Create color texture to receive the scene render
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Attach color texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    // 3. Create depth render buffer
    glGenRenderbuffers(1, &depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

    // Verify framebuffer completeness status
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create complete OpenGL framebuffer!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::cleanup() const {
    if (fbo) glDeleteFramebuffers(1, &fbo);
    if (colorTexture) glDeleteTextures(1, &colorTexture);
    if (depthRenderBuffer) glDeleteRenderbuffers(1, &depthRenderBuffer);
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(int w, int h) {
    if (width == w && height == h) return;
    width = w;
    height = h;
    cleanup();
    create(width, height);
}

} // namespace swarcs::accretion::graphics