#include "TextureFile.h"

#include <glad.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include "Logger.h"

TextureFile::TextureFile() {}

TextureFile::~TextureFile()
{
    if (mId) {
        glDeleteTextures(1, &mId);
    }
}

bool TextureFile::open(std::string filepath)
{
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        LOG("Unable to load image");
        return false;
    }

    mWidth = width;
    mHeight = height;

    glGenTextures(1, &mId);
    glBindTexture(GL_TEXTURE_2D, mId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return true;
}

uint32_t TextureFile::getId() const { return mId; }

uint32_t TextureFile::getWidth() const { return mWidth; }

uint32_t TextureFile::getHeight() const { return mHeight; }

void TextureFile::bind(uint32_t textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, mId);
}

void TextureFile::unbind() { glBindTexture(GL_TEXTURE_2D, 0);}
