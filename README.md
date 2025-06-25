# Conway's Game of Life with GLFW

This project is a C++ implementation of Conway's Game of Life with a modern interactive interface using GLFW and OpenGL.

## Features

- **Infinite grid**: Only living cells are stored, allowing unlimited exploration.
- **Drag & drop navigation**: Move the grid by holding the left mouse button and dragging.
- **Zoom**: Use the mouse wheel to zoom in/out, centered on the mouse pointer.
- **Cell editing**:
  - **Right click** to make a cell alive at the cursor position.
- **Simulation control**:
  - **Space bar** to start/stop the simulation.
- **Black grid on white background** for clear visibility.

## Requirements
- CMake
- GCC (MinGW on Windows) or any C++17 compiler
- [GLFW](https://www.glfw.org/) (automatically downloaded by CMake)

## Build
Open a terminal in the project folder and run:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

CMake will automatically download and build GLFW for your platform (no need to manually download binaries).

## Run
From the `build` folder:

```sh
./GLFW_Interface
```

## Cross-platform compatibility

- The project is tested and works on both **Windows** and **Linux**.
- On Linux, make sure you have OpenGL development libraries installed (e.g. `sudo apt install libgl1-mesa-dev xorg-dev`).
- The CMake configuration will automatically link the correct OpenGL library for your platform.
- All dependencies are handled by CMake FetchContent.

## Project structure

- `main.cpp`: entry point, main loop, user interaction
- `GameOfLife.hpp` / `GameOfLife.cpp`: infinite grid logic
- `Renderer.hpp` / `Renderer.cpp`: grid and cell rendering
- `Camera.hpp` / `Camera.cpp`: camera/viewport logic (pan & zoom)
- `CMakeLists.txt`: CMake configuration (uses FetchContent for GLFW)

## Customization

- Change window or cell size in `main.cpp`.
- Improve logic or rendering as you wish.

## License

This project is licensed under the [MIT License](./LICENSE) — free for personal or educational use.
