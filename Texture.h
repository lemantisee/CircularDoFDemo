#pragma once

#include <cstdint>

class Texture
{
public:
    Texture();
    ~Texture();

    bool init(uint32_t width, uint32_t height, uint32_t channels);
    uint32_t getId() const;
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    int getGlFormat() const;

    void bind(uint32_t textureUnit = 0);
    void unbind();

private:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    uint32_t mId = 0;
    int mChannels = 3;
};
