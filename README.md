# AccretionDesktop

**AccretionDesktop** is a modern, high-performance Linux desktop wallpaper engine built in **C++23** and **OpenGL ES 3.0**, designed to render dynamic, real-time shader animations seamlessly across multi-monitor setups (spanned displays).

---

## Architecture & Design Principles

The project strictly follows clean architecture, polymorphism, and **SOLID** principles, separating platform infrastructure, graphics contexts, and rendering logic:

* **Dependency Injection (DI)**: Core subsystems (`IWindow`, `IGraphicsContext`, `IRenderable`, `IRenderer`) are injected into the application loop (`AccretionApp`), ensuring high testability and modularity.
* **RAII (Resource Acquisition Is Initialization)**: All system handles (X11 displays, windows, EGL contexts, GPU resource wrappers via `ProgramHandle`/`ShaderHandle`) are safely managed via standard smart pointers and RAII wrappers.
* **Multi-Monitor Spanning**: Automatically detects physical monitors using the **X11 XRandR** extension to generate a unified high-resolution wallpaper canvas spanning all connected screens.

---

## Tech Stack & Dependencies

* **Language**: C++23
* **Build System**: CMake (Minimum version 3.25)
* **Graphics API**: OpenGL ES 3.0, EGL (GLSL ES 300 shaders)
* **Windowing Systems**: X11 (with XRandR support), Wayland (experimental groundwork via `WaylandSurface`)
* **Required System Libraries**:
  * `libX11`, `libXrandr`
  * `libEGL`, `libGLESv2`, `libGLX`, `opengl`
  * `wayland-client`, `wayland-egl`

---

## Build & Run Instructions

### 1. Install Dependencies (Ubuntu / Debian / Arch)
Ensure you have a C++23 compliant compiler (GCC 13+ or Clang 16+) and development headers installed:
```bash
# Ubuntu/Debian
sudo apt install build-essential cmake libx11-dev libxrandr-dev libegl1-mesa-dev libgles2-mesa-dev libwayland-dev
```

### 2. Clone and Build
```bash
git clone [https://github.com/your-username/accretion-desktop.git](https://github.com/your-username/accretion-desktop.git)
cd accretion-desktop
```

#### Create build directory and configure with CMake
```bash
cmake -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
```

#### Compile the project
```bash
cmake --build cmake-build-debug --target AccretionDesktop -j$(nproc)
```

### 3. Run the Application
   Execute the compiled binary from the project root (so relative shader paths resolve correctly):

```bash
./cmake-build-debug/AccretionDesktop
```

Note: Press any key inside the window or send a termination signal to exit the wallpaper loop.

Project Structure

```
accretion-desktop/
├── CMakeLists.txt
├── main.cpp
├── README.md
├── shaders/
│   ├── fragment.glsl
│   └── vertex.glsl
├── include/
│   └── swarcs/
│       └── accretion/
│           ├── app/
│           │   └── AccretionApp.hpp
│           ├── graphics/
│           │   ├── FrameContext.hpp
│           │   ├── FullScreenQuad.hpp
│           │   ├── IRenderable.hpp
│           │   ├── IRenderer.hpp
│           │   ├── OpenGLRenderer.hpp
│           │   ├── ProgramHandle.hpp
│           │   ├── ShaderHandle.hpp
│           │   └── ShaderManager.hpp
│           └── platform/
│               ├── EGLManager.hpp
│               ├── IGraphicsContext.hpp
│               ├── IWindow.hpp
│               ├── WaylandSurface.hpp
│               └── X11Window.hpp
└── src/
    └── swarcs/
        └── accretion/
            ├── app/
            │   └── AccretionApp.cpp
            ├── graphics/
            │   ├── FullScreenQuad.cpp
            │   ├── ProgramHandle.cpp
            │   ├── ShaderHandle.cpp
            │   └── ShaderManager.cpp
            └── platform/
                ├── EGLManager.cpp
                ├── WaylandSurface.cpp
                └── X11Window.cpp
```

Author
Boško Mijin (@author Bosko Mijin)

Since: 2026-02