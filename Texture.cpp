#include "Texture.h"

#include <glad.h>

#include "Logger.h"

Texture::Texture() {}

Texture::~Texture()
{
    if (mId) {
        glDeleteTextures(1, &mId);
    }
}

bool Texture::init(uint32_t width, uint32_t height, uint32_t channels)
{
    mWidth = width;
    mHeight = height;
    mChannels = channels;

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int internalFormat = GL_RGBA;
    int format = GL_RGB;

    switch (channels) {
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, nullptr);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        LOG_ERROR("Unable to create texture");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

uint32_t Texture::getId() const { return mId; }

uint32_t Texture::getWidth() const { return mWidth; }

uint32_t Texture::getHeight() const { return mHeight; }

int Texture::getGlFormat() const
{
    switch (mChannels) {
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default: break;
    }

    return GL_RGB;
}

void Texture::bind(uint32_t textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, mId);
}

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0);}
