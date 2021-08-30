cmake_minimum_required(VERSION 3.0)
project(Lines)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_subdirectory(lib/glad/)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIR})

set(SOURCE_FILES
    app.cpp
    renderer.cpp
    main.cpp
)

add_executable(Lines WIN32 ${SOURCE_FILES})
target_link_libraries(Lines ${OPENGL_LIBRARY} ${SDL2_LIBRARY} glad)