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
    // if (ImGui::Begin("Test")){
        ImGui::Image((ImTextureID)(intptr_t)mImageTexture->getId(), ImVec2(512, 512));
        ImGui::SameLine();
        ImGui::Image((ImTextureID)(intptr_t)mFilterTexture->getId(), ImVec2(512, 512));
        ImGui::SameLine();
        ImGui::Image((ImTextureID)(intptr_t)mTextureBlur->getId(), ImVec2(512, 512));

        ImGui::Image((ImTextureID)(intptr_t)mTextureCompR->getId(), ImVec2(512, 512));
        ImGui::SameLine();
        ImGui::Image((ImTextureID)(intptr_t)mTextureCompG->getId(), ImVec2(512, 512));
        ImGui::SameLine();
        ImGui::Image((ImTextureID)(intptr_t)mTextureCompB->getId(), ImVec2(512, 512));
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
    if (!mImageTexture->open("color1.png")) {
        LOG_ERROR("Unable to load texture file");
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

    mTextureCompG = std::make_unique<Texture>();
    if (!mTextureCompG->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp G");
        return;
    }

    mTextureCompB = std::make_unique<Texture>();
    if (!mTextureCompB->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture comp B");
        return;
    }

    mTextureBlur = std::make_unique<Texture>();
    if (!mTextureBlur->init(mFilterTexture->getWidth(), mFilterTexture->getHeight(), 4)) {
        LOG_ERROR("Unable to init texture blur");
        return;
    }

    mRT = std::make_unique<RenderTarget>();
    // if (!mRT->init(mImageTexture->getWidth(), mImageTexture->getHeight())) {
    //     LOG_ERROR("Unable to init render target");
    //     return;
    // }

    mFilterShader = std::make_unique<GpuProgram>();
    if (!mFilterShader->build("filter.glsl")) {
        LOG_ERROR("Unable to init filter shader");
        return;
    }

    mCompRShader = std::make_unique<GpuProgram>();
    if (!mCompRShader->build("compR.glsl")) {
        LOG_ERROR("Unable to init compR shader");
        return;
    }

    mCompGShader = std::make_unique<GpuProgram>();
    if (!mCompGShader->build("compG.glsl")) {
        LOG_ERROR("Unable to init comp B shader");
        return;
    }

    mCompBShader = std::make_unique<GpuProgram>();
    if (!mCompBShader->build("compB.glsl")) {
        LOG_ERROR("Unable to init comp G shader");
        return;
    }

    mMergeShader = std::make_unique<GpuProgram>();
    if (!mMergeShader->build("merge.glsl")) {
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
    renderFilter();
    renderComp(*mTextureCompR, *mCompRShader);
    renderComp(*mTextureCompG, *mCompGShader);
    renderComp(*mTextureCompB, *mCompBShader);
    mergeImage();
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
    mImageTexture->bind();
    mMesh->draw();

    mRT->endDraw();
}

void Application::renderComp(const Texture &targetTexture, GpuProgram &shader)
{
    if (!mRT->attachTexture(targetTexture)) {
        LOG_ERROR("Unable to attach comp texture");
        return;
    }

    mRT->beginDraw();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.bind();
    mFilterTexture->bind();
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

    mMergeShader->bind();

    mFilterTexture->bind(0);
    mTextureCompR->bind(1);
    mTextureCompG->bind(2);
    mTextureCompB->bind(3);

    mMesh->draw();

    mRT->endDraw();
}
