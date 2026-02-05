# Graphics Renderer

C++ graphics renderer with ray tracing and sphere objects. This project uses Catch2 unit testing.

## Requirements

- CMake 3.22+
- A C++20-compatible compiler
- vcpkg (used via 'CMakePresets.json')

## Building the Project

This project uses CMake Presets
- cmake --preset=default (default configuration)
- cmake --build . (in build folder to build the project)
- navigate to executables in build floder (e.g. buildVCPkg) to run