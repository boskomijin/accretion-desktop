cmake_minimum_required(VERSION 3.25)
project(AccretionDesktop LANGUAGES CXX)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

# Optional / Platform packages using pkg-config if available, or direct discovery
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GLESV2 REQUIRED glesv2)

# Define executable target
add_executable(AccretionDesktop
        main.cpp
        src/swarcs/accretion/app/AccretionApp.cpp
        src/swarcs/accretion/platform/X11Window.cpp
        src/swarcs/accretion/platform/EGLManager.cpp
        src/swarcs/accretion/platform/WaylandSurface.cpp
        src/swarcs/accretion/graphics/ShaderManager.cpp
        src/swarcs/accretion/graphics/ShaderHandle.cpp
        src/swarcs/accretion/graphics/ProgramHandle.cpp
        src/swarcs/accretion/graphics/FullScreenQuad.cpp
)

# Include directories configuration
target_include_directories(AccretionDesktop PRIVATE
        include
        ${X11_INCLUDE_DIR}
        ${WAYLAND_CLIENT_INCLUDE_DIRS}
        ${WAYLAND_EGL_INCLUDE_DIRS}
        ${EGL_INCLUDE_DIRS}
        ${GLESV2_INCLUDE_DIRS}
)

# Modern target-based link libraries
target_link_libraries(AccretionDesktop PRIVATE
        X11
        X11::Xrandr
        OpenGL::OpenGL
        OpenGL::GLES2
        ${EGL_LIBRARIES}
        ${WAYLAND_CLIENT_LIBRARIES}
        ${WAYLAND_EGL_LIBRARIES}
)

# Compiler warnings and safety flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(AccretionDesktop PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wnon-virtual-dtor
    )
endif()