# vec3 Library

C++ vector math library implementing a 3-component vector 'vec3' and using Catch2 unit testing.

## Requirements

- CMake 3.22+
- A C++20-compatible compiler
- vcpkg (used via 'CMakePresets.json')

## Building the Project

This project uses CMake Presets
- cmake --preset=default (default configuration)
- cmake --build . (in build folder to build the project)
- navigate to executables in build floder (e.g. buildVCPkg) to run