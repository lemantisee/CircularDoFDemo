#pragma once

#include <memory>

struct GLFWwindow;
class TextureFile;
class Texture;
class RenderTarget;
class GpuProgram;
class QuadMesh;

class Application
{
public:
    Application();
    ~Application();

    void run();

private:
    enum Component {
        RedChannel = 0,
        GreenChannel = 1,
        BlueChannel = 2,
    };

    GLFWwindow *createWindow() const;
    void processInput();

    void initUI();
    void drawUI();
    void initTextures();
    void drawBlur();

    void renderFilter();
    void renderComp(const Texture &targetTexture, Component comp);
    void renderRgb(Texture &sourceTexture, const Texture &targetTexture);
    void mergeImage();

    GLFWwindow *mWindow = nullptr;
    std::unique_ptr<TextureFile> mImageTexture;
    std::unique_ptr<TextureFile> mDepthTexture;
    std::unique_ptr<Texture> mFilterTexture;
    std::unique_ptr<Texture> mTextureCompR;
    std::unique_ptr<Texture> mTextureCompR_RGB;

    std::unique_ptr<Texture> mTextureCompG;
    std::unique_ptr<Texture> mTextureCompG_RGB;

    std::unique_ptr<Texture> mTextureCompB;
    std::unique_ptr<Texture> mTextureCompB_RGB;

    std::unique_ptr<Texture> mTextureBlur;
    std::unique_ptr<RenderTarget> mRT;
    std::unique_ptr<GpuProgram> mFilterShader;
    std::unique_ptr<GpuProgram> mHorizontalPassShader;
    std::unique_ptr<GpuProgram> mVertPassShader;
    std::unique_ptr<GpuProgram> mToRGBShader;
    std::unique_ptr<QuadMesh> mMesh;

    bool mShowChannelR = false;
    bool mShowChannelG = false;
    bool mShowChannelB = false;
    int mImageSize = 512;
    int mKernelRadius = 8;
    float mFilterSize = 0.5;
};
