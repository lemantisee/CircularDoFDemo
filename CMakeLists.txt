cmake_minimum_required(VERSION 3.16)

project(dof LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dof main.cpp
    Application.h Application.cpp
    TextureFile.h TextureFile.cpp
    Logger.h
    RenderTarget.h RenderTarget.cpp
    GpuProgram.h GpuProgram.cpp
    QuadMesh.h QuadMesh.cpp
    Texture.h Texture.cpp
)

add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    imgui
)

configure_file(img/color.png color.png COPYONLY)
configure_file(img/color1.png color1.png COPYONLY)
configure_file(img/color2.png color2.png COPYONLY)
configure_file(img/color3.png color3.png COPYONLY)
configure_file(img/depth.png depth.png COPYONLY)

configure_file(shaders/filter.glsl filter.glsl COPYONLY)
configure_file(shaders/horizontalPass.glsl horizontalPass.glsl COPYONLY)
configure_file(shaders/verticalPass.glsl verticalPass.glsl COPYONLY)
configure_file(shaders/to_rgb.glsl to_rgb.glsl COPYONLY)

configure_file(shaders/coc.glsl coc.glsl COPYONLY)
configure_file(shaders/blur.glsl blur.glsl COPYONLY)
configure_file(shaders/composite.glsl composite.glsl COPYONLY)

include(GNUInstallDirs)
install(TARGETS dof
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
