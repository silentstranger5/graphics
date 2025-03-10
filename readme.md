# SDL Graphics Library

This library aims to provide additional functionality for the (SDL library)[https://libsdl.org].

## About

This repository includes two libraries:

1. glm - math for computer graphics:
    - Vectors: addition, subtraction, multiplication, scaling, rotation, reflection, shearing, etc.
    - Matrices: addition, subtraction, multiplication, scaling, transposition, determinant, etc.
2. graphics - drawing primitives: ellipses, polygons and wireframe meshes

Three examples are included as well:

- Clock widget. Displays the current time.
- Cube wireframe mesh
- Pyramid wireframe mesh

## How To Build

Dependencies:

- [CMake](https://cmake.org)
- [VCPKG](https://vcpkg.io)

```bash
git clone https://github.com/silentstranger5/graphics
cd graphics
cmake -B build -S . --preset=vcpkg
cmake --build build
```

## How To Use

```bash
render <model>
    model - clock, cube, pyramid
```
