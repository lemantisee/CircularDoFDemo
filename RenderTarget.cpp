#include "RenderTarget.h"

#include <glad.h>

#include "Texture.h"
#include "Logger.h"

RenderTarget::RenderTarget() {}

RenderTarget::~RenderTarget()
{
    if (mId) {
        glDeleteFramebuffers(1, &mId);
    }
}

// bool RenderTarget::init(uint32_t width, uint32_t height)
// {
//     mWidth = width;
//     mHeight = height;

//     glGenFramebuffers(1, &mId);
//     glBindFramebuffer(GL_FRAMEBUFFER, mId);

//     glGenTextures(1, &mTextureId);
//     glBindTexture(GL_TEXTURE_2D, mTextureId);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

//     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTextureId, 0);

//     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//         LOG_ERROR("Unable to create framebuffer");
//         return false;
//     }

//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
//     return true;
// }

bool RenderTarget::attachTexture(const Texture &texture)
{
    if (!mId) {
        glGenFramebuffers(1, &mId);
    }

    mWidth = texture.getWidth();
    mHeight = texture.getHeight();

    glBindFramebuffer(GL_FRAMEBUFFER, mId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getId(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR("Unable to create framebuffer");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void RenderTarget::beginDraw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mId);
    glViewport(0, 0, mWidth, mHeight);
}

void RenderTarget::endDraw() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
