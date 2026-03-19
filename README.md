# Graphics Renderer

C++ graphics renderer with ray tracing, shading, and multiple objects. This project uses Catch2 unit testing.

- Shapes: Spheres, Triangles
- Shaders: Lambertian, Blinn-Phong, Mirror, Diffuse, Emitter
- Additional Features: OpenMP Multithreading, JSON Scene Parsing, Bounding Volume       Hierarchy data structure

## Requirements

- CMake 3.22+
- A C++20-compatible compiler
- vcpkg (used via 'CMakePresets.json')

## Building the Project

This project uses CMake Presets
- cmake --preset=default (default configuration)
- cmake --build . (in build folder to build the project)

## Run the Project
- navigate to build folder (e.g. buildVCPkg/src) to run main.exe
