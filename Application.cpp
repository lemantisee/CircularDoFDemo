#include "Application.h"

#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "TextureFile.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "GpuProgram.h"
#include "QuadMesh.h"
#include "Logger.h"

Application::Application()
{
    mWindow = createWindow();
    initUI();
    initTextures();
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (mWindow) {
        glfwTerminate();
    }
}

void Application::run()
{
    while (!glfwWindowShouldClose(mWindow)) {
        processInput();

        drawBlur();
        drawUI();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}

GLFWwindow *Application::createWindow() const
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 960, "DOF", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return nullptr;
    }

    return window;
}

void Application::processInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mWindow, true);
    }
}

void Application::initUI()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init();
}

void Application::drawUI()
{
    int width = 0;
    int height = 0;

    glfwGetWindowSize(mWindow, &width, &height);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = {float(width), float(height)};


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    if (ImGui::Begin("#root", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration )){

        static bool showDepth = false;
        if (ImGui::Begin("View", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // ImGui::Checkbox("Show R", &mShowChannelR);
            // ImGui::SameLine();
            // ImGui::Checkbox("Show G", &mShowChannelG);
            // ImGui::SameLine();
            // ImGui::Checkbox("Show B", &mShowChannelB);
            ImGui::Checkbox("Show Depth", &showDepth);
            // ImGui::SliderInt("Image size", &mImageSize, 512, 1024);
            // ImGui::SliderFloat("Filter size", &mFilterSize, 0.1f, 3.0f);

            ImGui::SliderFloat("Near", &mNearField, 0.1f, 15.f);
            ImGui::SliderFloat("Far", &mFarField, 0.1f, 15.f);
            ImGui::SliderFloat("Focal depth", &mFocalDepth, 0.1f, 15.f);
            ImGui::SliderFloat("Max blur", &mMaxBlur, 1.0f, 30.f);
        }

        ImGui::End();

        ImVec2 imageSize = ImVec2(mImageSize, mImageSize);

        ImGui::Image((ImTextureID)(intptr_t)mImageTexture->getId(), imageSize);
        ImGui::SameLine();
        ImGui::Image((ImTextureID)(intptr_t)mTextureCoc->getId(), imageSize);
        ImGui::Image((ImTextureID)(intptr_t)mTextureBlur->getId(), imageSize);
        ImGui::SameLine();
        ImGui::Image((ImTextureID)(intptr_t)mTextureComposite->getId(), imageSize);

        // if (mShowChannelR) {
        //     ImGui::Image((ImTextureID)(intptr_t)mTextureCompR_RGB->getId(), imageSize);
        //     ImGui::SameLine();
        // }

        // if (mShowChannelG) {
        //     ImGui::Image((ImTextureID)(intptr_t)mTextureCompG_RGB->getId(), imageSize);
        //     ImGui::SameLine();
        // }

        // if (mShowChannelB) {
        //     ImGui::Image((ImTextureID)(intptr_t)mTextureCompB_RGB->getId(), imageSize);
        // }

        if (showDepth) {
            ImGui::Image((ImTextureID)(intptr_t)mDepthTexture->getId(), imageSize);
        }
    }

    ImGui::End();

    ImGui::Render();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::initTextures()
{
    mImageTexture = std::make_unique<TextureFile>();
    if (!mImageTexture->open("color3.png")) {
        LOG_ERROR("Unable to load color texture file");
        return;
    }

    mDepthTexture = std::make_unique<TextureFile>();
    if (!mDepthTexture->open("depth.png")) {
        LOG_ERROR("Unable to load depth texture file");
        return;
    }

    mFilterTexture = std::make_unique<Texture>();
    if (!mFilterTexture->init(mImageTexture->getWidth(), mImageTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture filter");
        return;
    }

    mTextureCompR = std::make_unique<Texture>();
    if (!mTextureCompR->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp R");
        return;
    }

    mTextureCompR_RGB = std::make_unique<Texture>();
    if (!mTextureCompR_RGB->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp R");
        return;
    }

    mTextureCompG = std::make_unique<Texture>();
    if (!mTextureCompG->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp G");
        return;
    }

    mTextureCompG_RGB = std::make_unique<Texture>();
    if (!mTextureCompG_RGB->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp R");
        return;
    }

    mTextureCompB = std::make_unique<Texture>();
    if (!mTextureCompB->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp B");
        return;
    }

    mTextureCompB_RGB = std::make_unique<Texture>();
    if (!mTextureCompB_RGB->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp R");
        return;
    }

    mTextureBlur = std::make_unique<Texture>();
    if (!mTextureBlur->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture blur");
        return;
    }

    mTextureBlurH = std::make_unique<Texture>();
    if (!mTextureBlurH->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture blur");
        return;
    }

    mTextureCoc = std::make_unique<Texture>();
    if (!mTextureCoc->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture blur");
        return;
    }

    mTextureComposite = std::make_unique<Texture>();
    if (!mTextureComposite->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture blur");
        return;
    }

    mRT = std::make_unique<RenderTarget>();

    mFilterShader = std::make_unique<GpuProgram>();
    if (!mFilterShader->build("filter.glsl")) {
        LOG_ERROR("Unable to init filter shader");
        return;
    }

    mHorizontalPassShader = std::make_unique<GpuProgram>();
    if (!mHorizontalPassShader->build("horizontalPass.glsl")) {
        LOG_ERROR("Unable to init compR shader");
        return;
    }

    mVertPassShader = std::make_unique<GpuProgram>();
    if (!mVertPassShader->build("verticalPass.glsl")) {
        LOG_ERROR("Unable to init merge shader");
        return;
    }

    mToRGBShader = std::make_unique<GpuProgram>();
    if (!mToRGBShader->build("to_rgb.glsl")) {
        LOG_ERROR("Unable to init merge shader");
        return;
    }

    mCocShader = std::make_unique<GpuProgram>();
    if (!mCocShader->build("coc.glsl")) {
        LOG_ERROR("Unable to init merge shader");
        return;
    }

    mBlurShader = std::make_unique<GpuProgram>();
    if (!mBlurShader->build("blur.glsl")) {
        LOG_ERROR("Unable to init merge shader");
        return;
    }

    mCompositeShader = std::make_unique<GpuProgram>();
    if (!mCompositeShader->build("composite.glsl")) {
        LOG_ERROR("Unable to init merge shader");
        return;
    }

    mMesh = std::make_unique<QuadMesh>();
    if (!mMesh->init()) {
        LOG_ERROR("Unable to init mesh");
        return;
    }
}

void Application::drawBlur()
{
    renderCoc();
    renderBlurH();
    renderBlurV();
    renderComposite();
    // renderFilter();

    // mHorizontalPassShader->bind();
    // mHorizontalPassShader->setInt("kernelRadius", mKernelRadius);
    // mHorizontalPassShader->unbind();

    // renderComp(*mTextureCompR, RedChannel);
    // renderComp(*mTextureCompG, GreenChannel);
    // renderComp(*mTextureCompB, BlueChannel);
    // mergeImage();

    // if (mShowChannelR) {
    //     renderRgb(*mTextureCompR, *mTextureCompR_RGB);
    // }

    // if (mShowChannelG) {
    //     renderRgb(*mTextureCompG, *mTextureCompG_RGB);
    // }

    // if (mShowChannelB) {
    //     renderRgb(*mTextureCompB, *mTextureCompB_RGB);
    // }
}

void Application::renderFilter()
{
    if (!mRT->attachTexture(*mFilterTexture)) {
        LOG_ERROR("Unable to attach filter texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mFilterShader->bind();
    mFilterShader->setInt("kernelRadius", mKernelRadius);
    mFilterShader->setFloat("filterSize", mFilterSize);
    mImageTexture->bind();
    mMesh->draw();

    mRT->endDraw();
}

void Application::renderComp(const Texture &targetTexture, Component comp)
{
    if (!mRT->attachTexture(targetTexture)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mHorizontalPassShader->bind();
    mHorizontalPassShader->setInt("component", comp);
    mHorizontalPassShader->setFloat("filterRadius", mFilterSize);
    mFilterTexture->bind(0);
    mDepthTexture->bind(1);
    mMesh->draw();

    mRT->endDraw();
}

void Application::renderRgb(Texture &sourceTexture, const Texture &targetTexture)
{
    if (!mRT->attachTexture(targetTexture)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    sourceTexture.bind();
    mMesh->draw();

    mRT->endDraw();
}

void Application::mergeImage()
{
    if (!mRT->attachTexture(*mTextureBlur)) {
        LOG_ERROR("Unable to attach blur texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mVertPassShader->bind();
    mVertPassShader->setFloat("filterRadius", mFilterSize);

    mFilterTexture->bind(0);
    mTextureCompR->bind(1);
    mTextureCompG->bind(2);
    mTextureCompB->bind(3);
    mDepthTexture->bind(4);

    mMesh->draw();

    mRT->endDraw();
}

void Application::renderCoc()
{
    if (!mRT->attachTexture(*mTextureCoc)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mCocShader->bind();

    mDepthTexture->bind();
    mCocShader->setFloat("focalDepth", mFocalDepth);
    mCocShader->setFloat("near",mNearField);
    mCocShader->setFloat("far", mFarField);
    // mCocShader->setFloat("maxBlur", 10.0f);

    mMesh->draw();

    mRT->endDraw();
}

void Application::renderBlurH()
{
    if (!mRT->attachTexture(*mTextureBlurH)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mBlurShader->bind();

    mImageTexture->bind(0);
    mTextureCoc->bind(1);
    mBlurShader->setInt("horizontal", 1);
    mBlurShader->setFloat("maxBlur",mMaxBlur);

    mMesh->draw();

    mRT->endDraw();
}

void Application::renderBlurV()
{
    if (!mRT->attachTexture(*mTextureBlur)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mBlurShader->bind();

    mTextureBlurH->bind(0);
    mTextureCoc->bind(1);
    mBlurShader->setInt("horizontal", 0);
    mBlurShader->setFloat("maxBlur",mMaxBlur);

    mMesh->draw();

    mRT->endDraw();
}

void Application::renderComposite()
{
    if (!mRT->attachTexture(*mTextureComposite)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mCompositeShader->bind();

    mImageTexture->bind(0);
    mTextureBlur->bind(1);
    mTextureCoc->bind(2);

    mMesh->draw();

    mRT->endDraw();
}
