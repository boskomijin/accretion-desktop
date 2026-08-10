cmake_minimum_required(VERSION 3.25)
project(AccretionDesktop CXX)

# Set C++23 standard and enforce strict compliance
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find mandatory external packages and graphics libraries
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

# Define the main executable target and list all source and header files
add_executable(AccretionDesktop
        main.cpp
        src/swarcs/accretion/app/AccretionApp.cpp
        include/swarcs/accretion/app/AccretionApp.hpp
        src/swarcs/accretion/platform/X11Window.cpp
        include/swarcs/accretion/platform/X11Window.hpp
        include/swarcs/accretion/platform/IWindow.hpp
        src/swarcs/accretion/platform/EGLManager.cpp
        include/swarcs/accretion/platform/EGLManager.hpp
        include/swarcs/accretion/platform/IGraphicsContext.hpp
        src/swarcs/accretion/platform/WaylandSurface.cpp
        include/swarcs/accretion/platform/WaylandSurface.hpp
        src/swarcs/accretion/graphics/ShaderManager.cpp
        include/swarcs/accretion/graphics/ShaderManager.hpp
        src/swarcs/accretion/graphics/FullScreenQuad.cpp
        include/swarcs/accretion/graphics/FullScreenQuad.hpp
        include/swarcs/accretion/graphics/IRenderable.hpp
        include/swarcs/accretion/graphics/ShaderHandle.hpp
        src/swarcs/accretion/graphics/ShaderHandle.cpp
        include/swarcs/accretion/graphics/ProgramHandle.hpp
        src/swarcs/accretion/graphics/ProgramHandle.cpp
        include/swarcs/accretion/graphics/FrameContext.hpp
        include/swarcs/accretion/graphics/IRenderer.hpp
        include/swarcs/accretion/graphics/OpenGLRenderer.hpp
)

# Link necessary platform, windowing, OpenGL ES, EGL, and Wayland libraries privately to the target
target_link_libraries(AccretionDesktop PRIVATE
        X11
        Xrandr
        EGL
        GLESv2
        GLX
        OpenGL
        wayland-client
        wayland-egl
)

# Specify private include directories for project headers and system package headers
target_include_directories(AccretionDesktop PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${X11_INCLUDE_DIR}
)