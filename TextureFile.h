#pragma once

#include <string>

class TextureFile
{
public:
    TextureFile();
    ~TextureFile();

    bool open(std::string filepath);
    uint32_t getId() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;

    void bind(uint32_t textureUnit = 0);
    void unbind();

private:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    uint32_t mId = 0;
};
