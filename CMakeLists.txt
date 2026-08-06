cmake_minimum_required(VERSION 3.20)
project(cpp-opengl-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Auto-collect engine sources so new .cpp files don't require touching this file
file(GLOB_RECURSE OGL3D_SOURCES 
    ${CMAKE_SOURCE_DIR}/ogl3d/source/*.cpp
)

# GLAD ships its loader as a .c file, so it needs to be compiled separately.
# Exclude glx (Linux/X11) sources: they don't apply to a Windows/WGL build.
file(GLOB_RECURSE GLAD_SOURCES_ALL
    ${CMAKE_SOURCE_DIR}/ogl3d/vendor/glad/src/*.c
)
list(FILTER GLAD_SOURCES_ALL EXCLUDE REGEX ".*glx.*")
set(GLAD_SOURCES ${GLAD_SOURCES_ALL})

add_executable(cpp-opengl-engine 
    game/main.cpp
    ${OGL3D_SOURCES}
    ${GLAD_SOURCES}
 "ogl3d/include/ogl3d/math/ovec4.h" "ogl3d/include/ogl3d/oprerequisites.h" "ogl3d/source/ogl3d/graphics/win32/cwin32graphicsengine.cpp" "ogl3d/include/ogl3d/math/orect.h" "ogl3d/include/ogl3d/graphics/overtexarrayobject.h" "ogl3d/source/ogl3d/graphics/overtexarrayobject.cpp" "ogl3d/include/ogl3d/graphics/oshaderprogram.h" "ogl3d/source/ogl3d/graphics/oshaderprogram.cpp" "ogl3d/include/ogl3d/graphics/ouniformbuffer.h" "ogl3d/source/ogl3d/graphics/ouniformbuffer.cpp" "ogl3d/include/ogl3d/math/omat4.h")

# Lets code do #include <ogl3d/...> and #include "owindow.h" without full paths
target_include_directories(cpp-opengl-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/ogl3d/include
    ${CMAKE_SOURCE_DIR}/ogl3d/source
    ${CMAKE_SOURCE_DIR}/ogl3d/vendor/glad/include
)

# Force wide-char Win32 API (CreateWindowExW, RegisterClassExW, etc.)
# to match the L"..." string literals used throughout the engine
target_compile_definitions(cpp-opengl-engine PRIVATE
    UNICODE
    _UNICODE
)

# Required by GLAD/OpenGL on Windows
target_link_libraries(cpp-opengl-engine PRIVATE opengl32)