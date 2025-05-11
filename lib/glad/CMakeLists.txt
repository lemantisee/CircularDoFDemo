project(glad)

add_library(${PROJECT_NAME} STATIC
    src/glad.c
    include/glad/glad.h
    include/KHR/khrplatform.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR
)

target_link_libraries(${PROJECT_NAME} PUBLIC opengl32.lib)
