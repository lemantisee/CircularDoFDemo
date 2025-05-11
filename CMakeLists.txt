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
configure_file(filter.glsl filter.glsl COPYONLY)
configure_file(compR.glsl compR.glsl COPYONLY)
configure_file(compG.glsl compG.glsl COPYONLY)
configure_file(compB.glsl compB.glsl COPYONLY)
configure_file(merge.glsl merge.glsl COPYONLY)

include(GNUInstallDirs)
install(TARGETS dof
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
