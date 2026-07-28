cmake_minimum_required(VERSION 3.20)
project(cpp-opengl-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Auto-collect engine sources so new .cpp files don't require touching this file
file(GLOB_RECURSE OGL3D_SOURCES 
    ${CMAKE_SOURCE_DIR}/ogl3d/source/*.cpp
)

add_executable(cpp-opengl-engine 
    game/main.cpp
    ${OGL3D_SOURCES}
)

# Lets code do #include <ogl3d/...> and #include "owindow.h" without full paths
target_include_directories(cpp-opengl-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/ogl3d/include
    ${CMAKE_SOURCE_DIR}/ogl3d/source
)

# Force wide-char Win32 API (CreateWindowExW, RegisterClassExW, etc.)
# to match the L"..." string literals used throughout the engine
target_compile_definitions(cpp-opengl-engine PRIVATE
    UNICODE
    _UNICODE
)