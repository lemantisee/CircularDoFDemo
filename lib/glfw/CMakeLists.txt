project(glfw)

add_library(${PROJECT_NAME} INTERFACE
    include/GLFW/glfw3.h
    include/GLFW/glfw3native.h
)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW
)

target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glfw3.lib)
