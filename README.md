# Graphics Renderer

C++ graphics renderer with ray tracing and rastrizing engines. This project uses Catch2 unit testing.

- Shapes: Spheres, Triangles
- Shaders: Lambertian, Blinn-Phong
- Ray Tracer Exclusive Shaders: Mirror, Diffuse, Emitter, Dielectric
- Additional Features: OpenMP Multithreading, JSON Scene Parsing, Bounding Volume       Hierarchy data structure, OpenGL implementation

## Ray Tracer Arguments

i - input file (json support only)
o - output file name
w - width of image
h - height of image
a - aspect ratio
r - strata for per pixel sampling (square root of number of rays per pixel e.g. -r 10 = 100 rays per pixel)
k - recursion depth

## Rasterizer Features

- Shader that supports lambertian and Blinn-Phong shading (ks = (0,0,0) for lambertian)
- OpenGL context window of scene
- Camera movement
- No shadows

## OpenGL Rasterizer Camera Controls

- W: forward
- A: left
- S: backward
- D: right
- Q: up
- Z: down

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
- main.exe - ray tracer
- glfwExample.exe - rasterizer
