project(imgui)

add_library(${PROJECT_NAME} STATIC
    imgui.h imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imconfig.h

    backends/imgui_impl_glfw.h backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.h backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3_loader.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
