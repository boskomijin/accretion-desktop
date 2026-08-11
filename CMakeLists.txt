cmake_minimum_required(VERSION 3.25)
project(AccretionDesktop LANGUAGES CXX)

# ==============================================================================
# C++ Standard Configuration
# ==============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================================================================
# Package Discovery and Dependencies
# ==============================================================================
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GLESV2 REQUIRED glesv2)

# ==============================================================================
# Target Definition
# ==============================================================================
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
        include/swarcs/accretion/physics/Constants.hpp
        include/swarcs/accretion/physics/Relativity.hpp
        include/swarcs/accretion/physics/Gravity.hpp
)

# ==============================================================================
# Include Directories
# ==============================================================================
target_include_directories(AccretionDesktop PRIVATE
        include
        ${X11_INCLUDE_DIR}
        ${WAYLAND_CLIENT_INCLUDE_DIRS}
        ${WAYLAND_EGL_INCLUDE_DIRS}
        ${EGL_INCLUDE_DIRS}
        ${GLESV2_INCLUDE_DIRS}
)

# ==============================================================================
# Link Libraries
# ==============================================================================
target_link_libraries(AccretionDesktop PRIVATE
        X11
        X11::Xrandr
        OpenGL::OpenGL
        OpenGL::GLES2
        ${EGL_LIBRARIES}
        ${WAYLAND_CLIENT_LIBRARIES}
        ${WAYLAND_EGL_LIBRARIES}
)

# ==============================================================================
# Compiler Warnings and Safety Options
# ==============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(AccretionDesktop PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wnon-virtual-dtor
    )
endif()

# ==============================================================================
# Asset Management: Shader Synchronization Target
# ==============================================================================
#
# @brief Custom target to synchronize the shaders directory into the build output.
# Ensures all GLSL files and include dependencies are accessible at runtime.
#
add_custom_target(CopyShaders ALL
COMMENT "Synchronizing shaders directory to build output..."
)

add_custom_command(TARGET CopyShaders POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/shaders
$<TARGET_FILE_DIR:AccretionDesktop>/shaders
)

# Enforce dependency so shaders are automatically copied before execution
add_dependencies(AccretionDesktop CopyShaders)