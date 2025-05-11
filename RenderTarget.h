#pragma once

#include <cstdint>

class Texture;

class RenderTarget
{
public:
    RenderTarget();
    ~RenderTarget();

    // bool init(uint32_t width, uint32_t height);

    // uint32_t getId() const;
    // uint32_t getWidth() const;
    // uint32_t getHeight() const;

    bool attachTexture(const Texture &texture);

    void beginDraw();
    void endDraw();

private:
    uint32_t mWidth = 0;
    uint32_t mHeight = 0;
    uint32_t mId = 0;
    // uint32_t mTextureId = 0;
};
